# EngineProject
 
## Introduction
Introducing DXHEngine, a C++ and Direct3D 12 powered graphic rendering engine designed for high-performance and versatility. With its Entity Component System (ECS) and intuitive scripting, it's an ideal platform for testing out diverse game concepts to life.

## Features
**Entity Component System (ECS)** 
 - An efficient and flexible architecture that allows for high performance and easy management of game objects and their behaviors.<br />

**Scripting System**
 - Inspired by Unity's MonoBehaviour, our scripting system enables developers to easily script game logic and behaviors, allowing for rapid development and iteration.<br />

**GameObject Framework**
 - Includes foundational components such as transform and rigidbody, providing the essential building blocks for game development.<br />

## Getting Started
### 1-Prerequisites
To work with this engine, you will need:
- Visual Studio: The engine is developed in C++ and relies on Visual Studio for development and debugging.
- DirectX 12: Essential for graphics rendering. Most modern PCs come with DirectX 12 pre-installed, but ensure your system has it.

### 2-Installation
* Clone or Download: Get the engine by cloning the repository or downloading it as a zip file.

* Open the Solution: Navigate to the downloaded folder and open the solution file (.sln) in Visual Studio.

## Usage
<sub>GameObject is basically a container and it's destroyed automatically</sub>
```cpp
#include "DXHScripting.h"
//TO CREATE A GAMEOBJECT
DXH::GameObject* go = DXH::GameObject::Create();
```

## Screenshots / Demo
![Game](https://github.com/GuilBlack/EngineProject/assets/92313152/431f6c10-e3ac-4b34-88a1-c5d3ea1baf6d)

## Contributors
* [lvocanson](https://github.com/lvocanson)
* [GuilBlack](https://github.com/GuilBlack)
* [ArthurChassaing](https://github.com/ArthurChassaing)
* [Juuunnne](https://github.com/Juuunnne)
---
