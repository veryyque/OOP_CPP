#!/bin/bash

labs=("lab0a" "lab0b" "lab1" "lab2" "lab3" "lab4")

git checkout -b main
echo "# OOP C++ Labs" > README.md
git add README.md
git commit -m "Initial commit"
git push -u origin main

for lab in "${labs[@]}"; do
    git checkout main
    git checkout -b "$lab"
    git rm -rf . 2>/dev/null
    git clean -fd
    cp -r "$lab"/* . 2>/dev/null
    cp -r "$lab"/.[!.]* . 2>/dev/null
    git add .
    git commit -m "Add $lab"
    git push -u origin "$lab"
done

git checkout main
