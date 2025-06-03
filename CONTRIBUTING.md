# Contributing to MuZero

We love your input! We want to make contributing to CAIS MuZero as easy and transparent as possible, whether it's:

- Reporting a bug
- Discussing the current state of the code
- Submitting a fix
- Proposing new features
- Becoming a maintainer

## Development Workflow

1. Fork the repo and create your branch from `main`
2. Make your changes
3. Run the formatting script:
   ```bash
   ./scripts/format.sh
   ```
   This will:
   - Clean up unused imports
   - Sort imports
   - Format code according to our style guide
   - Run tests to verify changes
4. If tests pass, commit your changes
5. Push to your fork
6. Submit a pull request

## Code Style

We use several tools to maintain consistent code style:

- [black](https://github.com/psf/black) for code formatting
- [isort](https://pycqa.github.io/isort/) for import sorting
- [autoflake](https://github.com/PyCQA/autoflake) for cleaning up imports
- [pylint](https://www.pylint.org/) for code quality
- [mypy](https://mypy.readthedocs.io/) for type checking

The formatting script (`scripts/format.sh`) will automatically apply most of these tools. For manual checks:

```bash
# Format code
black .

# Sort imports
isort .

# Check types
mypy .

# Run linter
pylint muzero/
```

## Pull Request Process

1. Update the README.md with details of changes if needed
2. Ensure all tests pass
3. Ensure code meets our style guidelines
4. The PR will be merged once you have the sign-off of at least one other developer

## Report bugs using GitHub's [issue tracker](https://github.com/carletonai/muzero/issues)
We use GitHub issues to track public bugs. Report a bug by [opening a new issue](https://github.com/carletonai/muzero/issues/new); it's that easy!

## Write bug reports with detail, background, and sample code

**Great Bug Reports** tend to have:

- A quick summary and/or background
- Steps to reproduce
- What you expected would happen
- What actually happens
- Other Notes

## License
By contributing, you agree that your contributions will be licensed under its MIT License. 