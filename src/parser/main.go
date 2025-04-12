package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"

	dem "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs"
	common "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs/common"
	events "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs/events"
	msgs2 "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs/msgs2"
)

var roundNumber int = 0
var stopParsing bool = false

type Frame struct {
	Tick         int     `json:"tick"`
	ActiveWeapon string  `json:"activeWeapon"`
	Armor        int     `json:"armor"`
	Health       int     `json:"health"`
	IsDefusing   bool    `json:"isDefusing"`
	IsDucking    bool    `json:"isDucking"`
	IsPlanting   bool    `json:"isPlanting"`
	IsReloading  bool    `json:"isReloading"`
	PositionX    float64 `json:"positionX"`
	PositionY    float64 `json:"positionY"`
	PositionZ    float64 `json:"positionZ"`
	Team         int     `json:"team"`
	VelocityX    float64 `json:"velocityX"`
	VelocityY    float64 `json:"velocityY"`
	// VelocityZ       float64 `json:"velocityZ"`
	ViewDirectionX float32 `json:"viewDirectionX"`
	// ViewDirectionY  float32 `json:"viewDirectionY"`
	Weapons []string `json:"weapons"`
}

type Player struct {
	Name      string  `json:"name"`
	SteamID   uint64  `json:"steamID"`
	DeathTick int     `json:"deathTick,omitempty"`
	Frames    []Frame `json:"frames"`
}

func main() {
	// Open the demo file
	f, err := os.Open("../demos/test1.dem")
	if err != nil {
		log.Fatalf("Failed to open demo file: %v", err)
	}
	defer f.Close()

	// Create a new parser instance
	p := dem.NewParser(f)
	defer p.Close()

	var roundFreezeEndTick int
	var roundScoreUpdateTick int
	var roundEndReason events.RoundEndReason
	var winningTeam common.Team
	var roundStartTick int
	var roundEndTick int
	var mapName string
	currentTick := 0

	fmt.Printf("%v", p.TickRate())

	playerMap := make(map[uint64]*Player)

	p.RegisterEventHandler(func(e events.RoundStart) {
		log.Printf("Round started")
		roundNumber++
		roundStartTick = currentTick
		fmt.Printf("Round number: %d\n", roundStartTick)
	})
	p.RegisterEventHandler(func(e events.RoundEnd) {
		stopParsing = true
		roundEndReason = e.Reason
		winningTeam = e.Winner
		roundEndTick = currentTick
		log.Printf("Round ended: %v", e.Reason)
	})

	p.RegisterEventHandler(func(e events.RoundFreezetimeEnd) {
		roundFreezeEndTick = currentTick
	})

	p.RegisterEventHandler(func(e events.ScoreUpdated) {
		roundScoreUpdateTick = currentTick
	})

	p.RegisterNetMessageHandler(func(m *msgs2.CSVCMsg_ServerInfo) {
		fmt.Println(m.GetMapName())
		mapName = m.GetMapName()
	})

	p.RegisterEventHandler(func(e events.PlayerInfo) {
		if e.Info.XUID != 0 {
			player := Player{
				Name:    e.Info.Name,
				SteamID: e.Info.XUID,
				Frames:  []Frame{},
			}
			playerMap[e.Info.XUID] = &player
		}
	})

	p.RegisterEventHandler(func(e events.Kill) {
		if e.Victim != nil {
			if Victim, exists := playerMap[e.Victim.SteamID64]; exists {
				Victim.DeathTick = currentTick - roundStartTick - 1
			}
		}
	})

	// Parse the header to access general information
	header, err := p.ParseHeader()
	if err != nil {
		log.Fatalf("Failed to parse demo header: %v", err)
	}
	fmt.Printf("Parsing demo from map: %s\n", header.MapName)

	// Open a JSON file for writing
	file, err := os.Create("player_data.json")
	if err != nil {
		log.Fatalf("Failed to create JSON file: %v", err)
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")

	// Iterate through the demo ticks
	for {
		if stopParsing {
			break
		}
		frame, err := p.ParseNextFrame()
		currentTick += 1
		if err != nil {
			break // End of demo
		}

		// Access the current game state
		gameState := p.GameState()
		// currentTick := gameState.IngameTick()

		// Iterate over all players
		for _, player := range gameState.Participants().Playing() {
			if player == nil || player.Entity == nil {
				continue // Skip if player or entity is nil
			}

			if player.IsAlive() {
				frame := Frame{
					Tick:         currentTick - roundStartTick - 1,
					ActiveWeapon: player.ActiveWeapon().String(),
					Armor:        player.Armor(),
					Health:       player.Health(),
					PositionX:    player.Position().X,
					PositionY:    player.Position().Y,
					PositionZ:    player.Position().Z,
					Team:         int(player.Team),
					VelocityX:    player.Velocity().X,
					VelocityY:    player.Velocity().Y,
					// VelocityZ:      player.Velocity().Z,
					ViewDirectionX: player.ViewDirectionX(),
					// ViewDirectionY: player.ViewDirectionY(),
					Weapons: extractWeapons(player),
				}

				if existingPlayer, exists := playerMap[player.SteamID64]; exists {
					existingPlayer.Frames = append(existingPlayer.Frames, frame)
				}
			}
		}

		if !frame {
			break
		}
	}
	// Create a map to store the player data
	meta := map[string]interface{}{
		"isTwoFloor":           mapName == "de_vertigo" || mapName == "de_nuke" || mapName == "de_train",
		"mapName":              mapName,
		"mapScale":             getMapScale(mapName),
		"posX":                 getPosX(mapName),
		"posY":                 getPosY(mapName),
		"fromTick":             roundStartTick,
		"toTick":               roundEndTick,
		"roundNumber":          roundNumber,
		"winnerTeam":           int(winningTeam),
		"winReason":            int(roundEndReason),
		"tickRate":             64,
		"lowerSectionBorderZ":  getLowerSectionBorderZ(mapName),
		"roundTime":            (roundEndTick - roundStartTick) / 64,
		"roundFreezeEndTick":   roundFreezeEndTick - roundStartTick - 1,
		"roundScoreUpdateTick": roundScoreUpdateTick - roundStartTick - 1,
		"score": []int{
			p.GameState().Team(common.TeamTerrorists).Score(),
			p.GameState().Team(common.TeamCounterTerrorists).Score(),
		},
	}

	output := map[string]interface{}{
		"meta":            meta,
		"playerPositions": playerMap,
	}

	// Write the player data to the JSON file
	if err := encoder.Encode(output); err != nil {
		log.Fatalf("Failed to write player data to JSON file: %v", err)
	}
}

// extractWeapons returns a slice of weapon names the player currently holds
func extractWeapons(player *common.Player) []string {
	var weapons []string
	for _, weapon := range player.Weapons() {
		weapons = append(weapons, weapon.String())
	}
	return weapons
}

// getLowerSectionBorderZ returns the lower section border Z value based on the map name
func getLowerSectionBorderZ(mapName string) float64 {
	switch mapName {
	case "de_vertigo":
		return 11700
	case "de_nuke":
		return -495
	case "de_train":
		return -50
	default:
		return 0
	}
}

func getMapScale(mapName string) float64 {
	switch mapName {
	case "de_mirage":
		return 5
	case "de_vertigo":
		return 4.0
	case "de_nuke":
		return 7
	case "de_train":
		return 4.082077
	case "de_inferno":
		return 4.9
	case "de_dust2":
		return 4.4
	case "de_anubis":
		return 5.22
	case "de_ancient":
		return 5
	case "de_overpass":
		return 5.2
	case "cs_office":
		return 4.1
	case "cs_italy":
		return 4.6
	case "ar_shoots":
		return 2.6875
	case "ar_baggage":
		return 2.539062
	default:
		return 1.0
	}
}

func getPosX(mapName string) float64 {
	switch mapName {
	case "de_mirage":
		return -3230
	case "de_vertigo":
		return -3168
	case "de_nuke":
		return -3453
	case "de_train":
		return -2308
	case "de_inferno":
		return -2087
	case "de_dust2":
		return -2476
	case "de_anubis":
		return -2796
	case "de_ancient":
		return -2953
	case "de_overpass":
		return -4831
	case "cs_office":
		return -1838
	case "cs_italy":
		return -2647
	case "ar_shoots":
		return -1368
	case "ar_baggage":
		return -1316
	default:
		return 0.0
	}
}

func getPosY(mapName string) float64 {
	switch mapName {
	case "de_mirage":
		return 1713
	case "de_vertigo":
		return 1762
	case "de_nuke":
		return 2887
	case "de_train":
		return 2078
	case "de_inferno":
		return 3870
	case "de_dust2":
		return 3239
	case "de_anubis":
		return 3328
	case "de_ancient":
		return 2164
	case "de_overpass":
		return 1781
	case "cs_office":
		return 1858
	case "cs_italy":
		return 2592
	case "ar_shoots":
		return 1952
	case "ar_baggage":
		return 1288
	default:
		return 0.0
	}
}
