#!/usr/bin/env bash

if [ -z "$VIRTUAL_ENV" ] && [ -z "$CONDA_PREFIX" ]; then
    echo "Error: Please activate your virtual environment (venv or conda) first"
    exit 1
fi

echo "Updating requirements.txt ... "
pip freeze | grep -v "^-e git" > requirements.txt

echo "Updating environment.yml ... "
cat > environment.yml << EOL
name: muzero
channels:
  - conda-forge
  - defaults
dependencies:
  - python=3.10
  - pip:
EOL

while IFS= read -r line; do
    echo "    - $line" >> environment.yml
done < requirements.txt

echo -e "Successfully updated environment files!" 