#!/usr/bin/env bash

set -e

echo "Running code formatting and quality checks..."

if ! command -v black &> /dev/null; then
    echo "Installing black..."
    pip install black
fi

    echo "Installing isort..."
    pip install isort
fi

if ! command -v autoflake &> /dev/null; then
    echo "Installing autoflake..."
    pip install autoflake
fi

echo "Cleaning up imports..."
autoflake --in-place --recursive --remove-all-unused-imports --remove-unused-variables muzero/

echo "Sorting imports..."
isort muzero/

echo "Formatting code with black..."
black muzero/

echo "Formatting complete!"
echo "Running tests..."
pytest