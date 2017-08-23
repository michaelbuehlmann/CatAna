#!/bin/bash
#if [ "$TRAVIS_BRANCH" == "prod" ]; then
  doctr deploy . --built-docs docs/_build/html
#fi
