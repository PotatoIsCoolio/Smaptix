#pragma once

#include <math.h>
#include <string.h>
#include <cctype> // needed for the text feature because of toupper
#define PiNumberio 3.14159 


// I will prob show how to use this with ImGui soon.
// Or maybe never idk.
// Feels great to be king of RecRoom.

/*
0 Grid/floor/flat
1 Pyramid 
2 Sphere 
3 Cube 
4 Wall
5 Floor
6 Spiral 
7 Staircase
8 DNA
9 Waves 
10 Rings 
11 Text 
12 Heart 
13 Helix
15 Tornado
*/


struct Il2CppString {void* Cwass; int STringLength; wchar_t chars[1];};

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
}; 

bool FoodSploitMod = false;

int FoodieCount = 1;
float FoodieSpacing = 1.0f;
float FoodSizeScale = 1.0f;

int FoodiesBuildMode = 0; // This is used for the casing in the switch statements that is below in my codie.
char FoodieTextInput[32] = "Made By PotatoIsCool"; // This is text that will spawn for case 11 (text)
const char* ABC_Bitmap[26] = { // a bit map I stole from StackOverflow its all capital letters and 5x5 pixels. 
    "0111010001100011111110001", // Its also A-Z only no numbers or special chars.
    "1111010001111101000111110",
    "0111010001100001000001110",
    "1111010001100011000111110",
    "1111110000111101000011111",
    "1111110000111101000010000",
    "0111110000101111000101111",
    "1000110001111111000110001",
    "1111100100001000010011111",
    "0000100001000011000101110",
    "1000110010111001001010001",
    "1000010000100001000011111",
    "1000111011101011000110001",
    "1000111001101011001110001",
    "0111010001100011000101110",
    "1111010001111101000010000",
    "0111010001100011010101101",
    "1111010001111101001010001",
    "0111110000011100000111110",
    "1111100100001000010000100",
    "1000110001100011000111110",
    "1000110001100010101000100",
    "1000110001101011111110001",
    "1000110001010100010110001",
    "1000110001010100010000100",
    "1111100010001000100011111" 
};

// This is the first ever custom building foodsploit feature. Skids are copying it and calling it their own.
// This hooks into the spawn consumable method.
// I also removed a few features due to them not being SFW. That includes the rocket (looks like a dick) and manji because that is used for the Buddhism culture
// int VALUE = ceil(sqrt(FoodieCount)); This just calculates of the square root of however many items you wanna spawn.
// And then the ceil just rounds it up to like tyhe closes whole number. Because the method will break im pretty sure.
// This was purely made by PotatoIsCool and my CSS bot for the math so please give credit!!! Also thank you.
// You will need to find the method yourself as it changes every update.
// You can find it by searching for the method "SpawnConsumable" in the il2cpp dump.
// ID is the food GUID. You can swap it if you would like.

void* (*Old_SpawnConsum)(void*, Il2CppString*, void*, Vector3, Quaternion, float, void*);
void* SpawnCumsHooker(void* MethodInits, Il2CppString* id, void* nullableType, Vector3 Posotion, Quaternion Rotation, float scale, void* spawnFlags) {
    if (!FoodSploitMod)
        return Old_SpawnConsum(MethodInits, id, nullableType, Posotion, Rotation, scale, spawnFlags);

    float spacing = FoodieSpacing * FoodSizeScale;
    int count = 0;

    switch (FoodiesBuildMode) { 
    case 0: { // Default grid thing. FLOOR
        for (int i = 0; i < FoodieCount; i++) {
            Vector3 offset = {Posotion.x + (i % 5) * spacing,Posotion.y,Posotion.z + (i / 5) * spacing};
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 1: { // The fucking thing nobody knows how it was build. Pyramid
        int baseSize = 1;
        while (true) {
            int layers = (baseSize + 1) / 2;
            int total = 0;
            for (int i = 0; i < layers; ++i) {
                int layerSize = baseSize - i * 2;
                total += layerSize * layerSize;
            }
            if (total > FoodieCount) {
                baseSize -= 2;
                break;
            }
            baseSize += 2;
        }
        if (baseSize < 1) baseSize = 1;
        int height = (baseSize + 1) / 2;
        for (int level = 0; level < height; level++) {
            int layerSize = baseSize - level * 2;
            float y = Posotion.y + level * spacing;
            for (int x = 0; x < layerSize; x++) {
                for (int z = 0; z < layerSize; z++) {
                    Vector3 offset = {
                        Posotion.x + (x - layerSize / 2) * spacing,
                        y, // KEEP THE HIGHT THE SAME UNLESS YOU WANT IT ABOVE HEAD POTATO
                        Posotion.z + (z - layerSize / 2) * spacing
                    };
                    Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
                }
            }
        }
        break;
    }

    case 2: { // Sphere
        int layers = ceil(cbrtf(FoodieCount)); // layers it estimates
        for (int i = 0; i < FoodieCount; i++) {
            float theta = (float)i / layers * PiNumberio;
            float phi = (float)i * 0.618f * 2 * PiNumberio;
            Vector3 offset = {
                Posotion.x + sin(theta) * cos(phi) * spacing * layers / 2,
                Posotion.y + cos(theta) * spacing * layers / 2,
                Posotion.z + sin(theta) * sin(phi) * spacing * layers / 2
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 3: { // Cube
        int edge = ceil(cbrtf(FoodieCount));
        for (int x = 0; x < edge; x++) {
            for (int y = 0; y < edge; y++) {
                for (int z = 0; z < edge; z++) {
                    if (++count > FoodieCount) break;
                    Vector3 offset = {
                        Posotion.x + (x - edge / 2) * spacing,
                        Posotion.y + (y - edge / 2) * spacing,
                        Posotion.z + (z - edge / 2) * spacing
                    };
                    Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
                }
            }
        }
        break;
    }

    case 4: { // Wall
        int width = ceil(sqrt(FoodieCount));
        for (int i = 0; i < FoodieCount; i++) {
            Vector3 offset = {
                Posotion.x + (i % width) * spacing,
                Posotion.y + (i / width) * spacing,
                Posotion.z
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 5: { // Floor
        int width = ceil(sqrt(FoodieCount));
        for (int i = 0; i < FoodieCount; i++) {
            Vector3 offset = {
                Posotion.x + (i % width) * spacing,
                Posotion.y,
                Posotion.z + (i / width) * spacing
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 6: { // Spiral
        float angle = 0.0f;
        float radius = 1.0f;
        for (int i = 0; i < FoodieCount; i++) {
            Vector3 offset = {
                Posotion.x + cos(angle) * radius,
                Posotion.y,
                Posotion.z + sin(angle) * radius
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
            angle += 0.4f;
            radius += 0.05f * spacing;
        }
        break;
    }

    case 7: { // Staircase
        for (int i = 0; i < FoodieCount; i++) {
            Vector3 offset = {
                Posotion.x + i * spacing,
                Posotion.y + i * spacing * 0.5f,
                Posotion.z
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 8: { // DNA // Make sure spacing is up too!
        for (int i = 0; i < FoodieCount / 2; i++) {
            float angle = i * 0.3f;
            float height = i * spacing * 0.5f;

            Vector3 offset1 = {
                Posotion.x + cos(angle) * spacing,
                Posotion.y + height,
                Posotion.z + sin(angle) * spacing
            };

            Vector3 offset2 = {
                Posotion.x - cos(angle) * spacing,
                Posotion.y + height,
                Posotion.z - sin(angle) * spacing
            };

            Old_SpawnConsum(MethodInits, id, nullableType, offset1, Rotation, FoodSizeScale, spawnFlags); // Line 1
            Old_SpawnConsum(MethodInits, id, nullableType, offset2, Rotation, FoodSizeScale, spawnFlags); // Line 2
        }
        break;
    }

    case 9: { // Waves Like ocean
        int width = ceil(sqrt(FoodieCount));
        for (int i = 0; i < FoodieCount; i++) {
            int x = i % width;
            int z = i / width;

            float yOffset = sinf(x * 0.5f + z * 0.3f) * spacing;

            Vector3 offset = {
                Posotion.x + x * spacing,
                Posotion.y + yOffset,
                Posotion.z + z * spacing
            };

            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 10: { // Rings
        int rings = FoodieCount / 10;
        for (int r = 0; r < rings; r++) {
            float radius = (r + 1) * spacing;
            for (int i = 0; i < 10; i++) {
                float angle = (i / 10.0f) * PiNumberio * 2.0f;
                Vector3 offset = {
                    Posotion.x + cosf(angle) * radius,
                    Posotion.y + r * spacing,
                    Posotion.z + sinf(angle) * radius
                };
                Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
            }
        }
        break;
    }

    case 11: { // Text
        float charSpacing = spacing * 6.0f;
        float pixelSize = spacing;

        int textLen = strlen(FoodieTextInput);
        int startX = -textLen * 3; // Center text

        for (int c = 0; c < textLen; c++) {
            char ch = toupper(FoodieTextInput[c]);
            if (ch < 'A' || ch > 'Z') continue;

            const char* bitmap = ABC_Bitmap[ch - 'A'];
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                    if (bitmap[row * 5 + col] == '1') {
                        Vector3 offset = {
                            Posotion.x + (startX + c * 6 + col) * pixelSize,
                            Posotion.y + (4 - row) * pixelSize,
                            Posotion.z
                        };
                        Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
                    }
                }
            }
        }
        break;
    }
    case 12: { // I love pillow :3
        for (int i = 0; i < FoodieCount; i++) {
            float t = (float)i / FoodieCount * 2 * PiNumberio;
            float x = 16 * powf(sinf(t), 3);
            float y = 13 * cosf(t) - 5 * cosf(2 * t) - 2 * cosf(3 * t) - cosf(4 * t);
            Vector3 offset = {
                Posotion.x + x * spacing * 0.1f,
                Posotion.y + y * spacing * 0.1f,
                Posotion.z
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }
    case 13: { // Racist shirt :3
        for (int i = 0; i < FoodieCount; i++) {
            float angle = i * 0.3f;
            Vector3 offset = {
                Posotion.x + cosf(angle) * spacing,
                Posotion.y + i * spacing * 0.2f,
                Posotion.z + sinf(angle) * spacing
            };
            Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        }
        break;
    }

    case 14: { // Rocket Removing this due to it looking like a dick
        Vector3 offset = {0,0,0};
        Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
        break;
    }

    case 15: { // Tornadoo 
        int height = 20; // PPkleaae adjust if needded or make a slider for it.
        float angle = 0.0f;

        for (int i = 0; i < height; i++) {
            float t = (float)i / height;
            float radius = spacing * (0.5f + t * 2.5f);

            for (int j = 0; j < 6; j++) {
                float armAngle = angle + j * PiNumberio / 3.0f;
                Vector3 offset = {
                    Posotion.x + cosf(armAngle) * radius,
                    Posotion.y + i * spacing * 0.5f,
                    Posotion.z + sinf(armAngle) * radius
                };
                Old_SpawnConsum(MethodInits, id, nullableType, offset, Rotation, FoodSizeScale, spawnFlags);
            }

            angle += 0.4f;
        }
        break;
    }
    return nullptr;
}
