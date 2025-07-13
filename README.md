# 🔢 OCR Sudoku Solver 🧩

[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![GTK+3](https://img.shields.io/badge/GUI-GTK%2B3-green.svg)](https://www.gtk.org/)
[![SDL2](https://img.shields.io/badge/Graphics-SDL2-red.svg)](https://www.libsdl.org/)

A powerful **Optical Character Recognition (OCR)** system that can detect, extract, and solve Sudoku puzzles from images! 📸✨

## 🌟 Features

- 🖼️ **Image Processing**: Advanced preprocessing with noise reduction, edge detection, and morphological operations
- 🔍 **Grid Detection**: Automatic Sudoku grid detection using Hough transforms and geometric analysis
- 🧠 **Neural Network OCR**: Custom neural network for digit recognition (0-9)
- ⚡ **Smart Solver**: Efficient backtracking algorithm to solve extracted Sudoku puzzles
- 🖥️ **Dual Interface**: Both command-line and GTK+ graphical user interface
- 🔄 **Image Rotation**: Automatic and manual image rotation capabilities
- 📊 **Visual Feedback**: Real-time visualization of processing steps and results

## 🏗️ Project Structure

```
📦 OCR/
├── 🖼️ processing/          # Image preprocessing and filters
├── 🔍 grid_detection/      # Grid detection algorithms
├── ✂️ Cut/                 # Cell extraction and segmentation
├── 🧠 neuralNetwork/       # Digit recognition neural network
├── 🔧 solver/              # Sudoku solving algorithms
├── 🛠️ utils/               # Helper functions and utilities
├── 🖥️ sudoku-ocr.c        # Command-line interface
├── 🪟 sudoku-ocr-gui.c     # GTK+ graphical interface
└── 🎨 graphic.glade        # GUI layout file
```

## 🚀 Quick Start

### Prerequisites

Make sure you have the following dependencies installed:

```bash
# Ubuntu/Debian
sudo apt-get install libgtk-3-dev libsdl2-dev libsdl2-image-dev pkg-config

# Fedora/RHEL
sudo dnf install gtk3-devel SDL2-devel SDL2_image-devel pkgconfig
```

### Building

```bash
# Clone and navigate to the project
cd OCR

# Build both CLI and GUI versions
make all

# Or build individually
make sudoku-ocr      # Command-line version
make sudoku-ocr-gui  # GUI version
```

### Usage

#### 🖥️ Command Line Interface
```bash
# Basic usage
./sudoku-ocr image.jpg

# With manual rotation
./sudoku-ocr image.jpg -r 15.5

# Show help
./sudoku-ocr -h
```

#### 🪟 Graphical Interface
```bash
# Launch GUI
./sudoku-ocr-gui

# Then:
# 1. Click "Select" to choose an image
# 2. Click "Apply" or press 's' to process
# 3. View the solved Sudoku in real-time!
```

## 🧠 How It Works

1. **📥 Image Input**: Load Sudoku image (JPEG, PNG supported)
2. **🔧 Preprocessing**: 
   - Convert to grayscale
   - Apply noise reduction and filtering
   - Enhance contrast and apply morphological operations
3. **🔍 Grid Detection**:
   - Edge detection using Canny algorithm
   - Line detection with Hough transforms
   - Square detection and perspective correction
4. **✂️ Cell Extraction**: 
   - Segment 9x9 grid into individual cells
   - Normalize cell images for digit recognition
5. **🧠 Digit Recognition**: 
   - Custom neural network trained on digit dataset
   - Recognize digits 1-9 (empty cells detected as 0)
6. **⚡ Sudoku Solving**: 
   - Validate extracted grid
   - Apply backtracking algorithm to solve
7. **📊 Result Display**: Show original and solved grids

## 🎯 Supported Image Formats

- 📸 JPEG (.jpg, .jpeg)
- 🖼️ PNG (.png)
- 🔳 BMP (.bmp)

## 🧪 Testing

The project includes comprehensive test images in the `images/` directory:

```bash
# Test with provided samples
./sudoku-ocr images/image_01.jpeg
./sudoku-ocr images/image_02.jpeg
# ... and more!
```

## 📁 Output Files

The solver generates several intermediate files:
- `result/`: Step-by-step image processing results
- `grid.save`: Extracted grid data
- `grid.save.result`: Solved grid output

## 🔧 Configuration

### Neural Network Training
The neural network can be retrained with custom datasets:
- Training data format: `neuralNetwork/data_set_final*.txt`
- Network architecture: Configurable in `neuralNetwork/nn.c`

### Processing Parameters
Adjust image processing parameters in:
- `processing/`: Filter strengths, thresholds
- `grid_detection/`: Detection sensitivity, geometric constraints

## 👥 Contributors

This project was developed by the **STEL** team:

- 👨‍💻 **Tristan Hette** (`tristan.hette`)
- 👨‍💻 **Sasha Reggiani** (`sasha.reggiani`) 
- 👨‍💻 **Ewan Schwaller** (`ewan.schwaller`)
- 👨‍💻 **Lorenzo Taalba** (`lorenzo.taalba`)

## 📜 License

This project is part of an academic assignment

## 🐛 Troubleshooting

**Common Issues:**

- **Build errors**: Ensure all dependencies are installed
- **Image not detected**: Try rotating the image or improving lighting
- **Poor recognition**: Ensure grid is clearly visible and well-lit
- **GUI not launching**: Check GTK+3 installation and glade file presence

## 🚀 Future Improvements

- [ ] 📱 Mobile app version
- [ ] 🌐 Web interface
- [ ] 🎯 Support for blurry/low-quality images
- [ ] 🔄 Real-time camera processing
- [ ] 📈 Performance optimizations
- [ ] 🎨 Enhanced GUI with themes

---

*Made with ❤️ by the STEL team* 🚀
