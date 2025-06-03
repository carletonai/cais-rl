from setuptools import find_packages, setup

setup(
    name="muzero",
    version="0.1.0",
    packages=find_packages(),
    install_requires=[
        "torch>=2.7.0",
        "numpy>=2.2.6",
    ],
    extras_require={
        "dev": [
            "pytest>=8.4.0",
            "pytest-cov>=6.1.1",
            "black>=22.0.0",
            "isort>=5.0.0",
            "autoflake>=2.0.0",
            "pylint>=2.17.0",
            "mypy>=1.0.0",
        ],
    },
    python_requires=">=3.10",
)
