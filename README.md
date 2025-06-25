# 🐍 OpenGL Snake Game

<p align="center">
  <img src="https://media.giphy.com/media/fL6nB9pt8rKqMZ8B4n/giphy.gif" alt="Snake Game GIF" width="500"/>
</p>

A modern C++ rendition of the classic **Snake Game**, built using **OpenGL**, bringing nostalgic gameplay with animated visuals and keyboard controls.

---

## 🎮 Features

- 🐍 Smooth Snake Movement
- 🍎 Random Food Generation
- 💥 Game Over on Collision
- 🧱 Borders & Obstacles (Optional)
- 🎨 Simple Graphics via OpenGL
- 🎹 Real-time Keyboard Input
- ⏱️ Frame-based Speed Control

---

## 🚀 Getting Started

### 🔧 Requirements

- `C++ Compiler` (g++, clang++, etc.)
- `OpenGL` Libraries
- `GLUT` or `FreeGLUT` (for windowing)
- `CMake` (optional, if using a build system)

---

### 🛠️ Build Instructions

<details>
<summary><strong>💻 Compile with g++ (Linux/Mac/WSL)</strong></summary>

```bash
g++ main.cpp -o SnakeGame -lGL -lGLU -lglut
./SnakeGame
````

</details>

<details>
<summary><strong>🪟 Compile on Windows (MinGW + FreeGLUT)</strong></summary>

1. Install [MinGW](http://www.mingw.org/)
2. Install [FreeGLUT binaries](http://freeglut.sourceforge.net/)
3. Then compile with:

```bash
g++ main.cpp -o SnakeGame -lfreeglut -lopengl32 -lglu32
SnakeGame.exe
```

</details>

---

## 📁 Project Structure


📦 snake2/
├── main.cpp          # Game logic and rendering
├── README.md         # Project documentation
├── assets/           # Optional: Textures or sounds
└── build/            # Output binary (after compilation)



## 🎮 Gameplay

* Use **Arrow Keys** to control the snake.
* Eat the 🍎 food to grow.
* Avoid colliding with the wall or yourself.
* Your score increases with each food eaten.

---

## 🖼️ Demo

<p align="center">
  <img src="https://media.giphy.com/media/xUOxf48FkD5dF2vHVe/giphy.gif" alt="Gameplay Demo" width="450"/>
</p>

> *(Optional: Replace with your own GIF by uploading to GitHub or using an external link)*

---

## 💡 Future Enhancements

* ✅ Add scoring system
* ✅ Add main menu and pause functionality
* ⏳ Add levels & difficulty modes
* 🔊 Add sound effects
* 🖼️ Texture-based UI

---

## 🤝 Contributing

Contributions, issues, and suggestions are welcome!

1. Fork this repo
2. Create a branch: `git checkout -b feature-name`
3. Commit changes: `git commit -m 'Add some feature'`
4. Push to branch: `git push origin feature-name`
5. Submit a pull request

---

## 📝 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

## 📬 Contact

📧 **Haseeb** – [github.com/Hasee10](https://github.com/Hasee10)
🌐 Project Link – [github.com/Hasee10/Open\_GL\_Snake\_Game](https://github.com/Hasee10/Open_GL_Snake_Game)

---

