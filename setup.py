import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="visoptslider",
    version="1.0",
    author="Yuki Koyama",
    author_email="yuki@koyama.xyz",
    description="Qt-based implementation of VisOpt Slider [UIST 2014]",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/yuki-koyama/visoptslider",
    packages=["python_package_src"],
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    install_requires=[
        "matplotlib",
        "numpy",
        "PySide2",
    ],
)
