#!/bin/bash

set -e

echo "Running code formatting and quality checks..."

if ! command -v black &> /dev/null; then
    echo "Installing black..."
    pip install black
fi

if ! command -v isort &> /dev/null; then
    echo "Installing isort..."
    pip install isort
fi

if ! command -v autoflake &> /dev/null; then
    echo "Installing autoflake..."
    pip install autoflake
fi

if ! command -v pylint &> /dev/null; then
    echo "Installing pylint..."
    pip install pylint
fi

if ! command -v mypy &> /dev/null; then
    echo "Installing mypy..."
    pip install mypy
fi

echo "Cleaning up imports..."
autoflake --in-place --recursive --remove-all-unused-imports --remove-unused-variables muzero/

echo "Sorting imports..."
isort muzero/

echo "Formatting code with black..."
black muzero/

echo "Running type checks..."
mypy muzero/

echo "Running linting..."
pylint --rcfile=setup.cfg muzero/

echo "Formatting complete!"
echo "Running tests..."
pytest