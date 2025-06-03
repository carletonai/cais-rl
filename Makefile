.PHONY: install format lint test clean update-env help

help:
	@echo "Available commands:"
	@echo "  make install    - Install the package in editable mode with all dependencies"
	@echo "  make format     - Format code and run quality checks"
	@echo "  make lint       - Run linting checks"
	@echo "  make test       - Run tests with coverage"
	@echo "  make clean      - Clean up cache files"
	@echo "  make update-env - Update requirements.txt and environment.yml"

install:
	pip install -e .
	pip install -r requirements.txt
	pip install pytest pytest-cov pylint black mypy isort autoflake

format:
	./scripts/format.sh

lint:
	pylint --rcfile=setup.cfg muzero/
	black . --check
	mypy .

test:
	pytest --cov=./ --cov-report=term-missing --cov-report=html

clean:
	rm -rf build/
	rm -rf dist/
	rm -rf *.egg-info
	rm -rf .pytest_cache/
	rm -rf .mypy_cache/
	rm -rf htmlcov/
	find . -type d -name "__pycache__" -exec rm -rf {} +
	find . -type f -name "*.pyc" -delete

update-env:
	./scripts/update_env.sh 