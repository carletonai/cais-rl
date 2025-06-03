# MuZero

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Environment Setup

This project supports both conda and venv. Choose your preferred method:

### Prerequisites
Navigate to the project directory:
```bash
cd path/to/muzero
```

### Option 1: Using Conda

1. Create the conda environment:
```bash
conda env create -f environment.yml
```

2. Activate the environment:
```bash
conda activate muzero
```

3. Install the package and dependencies:
```bash
make install
```

### Option 2: Using venv

1. Create a virtual environment:
```bash
python -m venv venv
```

2. Activate the environment:
- On macOS/Linux:
```bash
source venv/bin/activate
```
- On Windows:
```bash
.\venv\Scripts\activate
```

3. Install the package and dependencies:
```bash
make install
```

## Development Workflow

We provide several Makefile commands to help with development:

```bash
# Show all available commands
make help

# Format code and run quality checks
make format

# Run linting checks
make lint

# Run tests with coverage
make test

# Clean up cache files
make clean

# Update requirements.txt and environment.yml
make update-env
```

### Code Formatting
Before committing your changes, run:
```bash
make format
```

This will:
- Clean up unused imports
- Sort imports
- Format code according to our style guide
- Run type checks
- Run linting
- Run tests to verify changes

### Running Tests
To run the test suite with coverage:
```bash
make test
```

## Environment Management

### For Conda Users
- To deactivate the environment:
```bash
conda deactivate
```

- To update the environment after modifying environment.yml:
```bash
conda env update -f environment.yml
```

- To remove the environment:
```bash
conda env remove -n muzero
```

### For venv Users
- To deactivate the environment:
```bash
deactivate
```

- To remove the environment, simply delete the `venv` directory:
```bash
rm -rf venv  # On macOS/Linux
rmdir /s /q venv  # On Windows
```

## Updating Environment Files

After installing new packages, update both environment files:
```bash
make update-env
```

This will:
- Update `requirements.txt` with all pip packages
- Update `environment.yml` with Python version and pip packages 