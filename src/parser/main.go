package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"

	dem "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs"
	common "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs/common"
	events "github.com/markus-wa/demoinfocs-golang/v4/pkg/demoinfocs/events"
)

var roundActive int = 0
var stopParsing bool = false

func main() {
	// Open the demo file
	f, err := os.Open("../demos/test.dem")
	if err != nil {
		log.Fatalf("Failed to open demo file: %v", err)
	}
	defer f.Close()

	// Create a new parser instance
	p := dem.NewParser(f)
	defer p.Close()

	p.RegisterEventHandler(func(e events.RoundStart) {
		roundActive = roundActive + 1
		if roundActive == 2 {
			stopParsing = true
		}
		log.Printf("Round started")
	})
	p.RegisterEventHandler(func(e events.RoundEnd) {
		// roundActive = false
		log.Printf("Round ended: %s", e.Reason)
		// Uncomment the next line if you want to exit after one round.
		// os.Exit(0)
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

	var allPlayerData []map[string]interface{}

	// Iterate through the demo ticks
	for {
		if stopParsing {
			break
		}
		frame, err := p.ParseNextFrame()
		if err != nil {
			break // End of demo
		}

		// Access the current game state
		gameState := p.GameState()

		// Iterate over all players
		for _, player := range gameState.Participants().Connected() {
			if player == nil || player.Entity == nil {
				continue // Skip if player or entity is nil
			}

			if player.IsAlive() {
				// Extract relevant player data using accessor methods
				playerData := map[string]interface{}{
					"tick":           gameState.IngameTick(),
					"steamID":        player.SteamID64,
					"name":           player.Name,
					"health":         player.Health(),
					"armor":          player.Armor(),
					"positionX":      player.Position().X,
					"positionY":      player.Position().Y,
					"positionZ":      player.Position().Z,
					"viewDirectionX": player.ViewDirectionX(),
					"viewDirectionY": player.ViewDirectionY(),
					"velocityX":      player.Velocity().X,
					"velocityY":      player.Velocity().Y,
					"velocityZ":      player.Velocity().Z,
					"team":           player.Team,
					"isDucking":      player.IsDucking(),
					"isReloading":    player.IsReloading,
					"isDefusing":     player.IsDefusing,
					"isPlanting":     player.IsPlanting,
					"weapons":        extractWeapons(player),
					"activeWeapon":   player.ActiveWeapon().String(),
				}

				allPlayerData = append(allPlayerData, playerData)
			}
		}

		if !frame {
			break
		}
	}
	if err := encoder.Encode(allPlayerData); err != nil {
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