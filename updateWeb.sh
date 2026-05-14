#!/bin/bash

# 1. Zbuduj projekt webowy
./buildWeb.sh

# 2. Utwórz nową, pustą gałąź (niezależną od historii).
git checkout --orphan web-build

# 3. Wymuś dodanie plików buildu (-f jest konieczne, bo wcześniej dodaliśmy je do .gitignore!)
git add -f index.html index.js index.wasm index.data

# 4. Utwórz commit (będzie to jedyny commit na tej gałęzi)
git commit -m "Web version update"

# 5. Wypchnij na GitHuba z flagą -f (force).
# To nadpisze całą gałąź 'web-build' na serwerze, kasując poprzedni build i jego historię.
git push -f origin web-build

# 6. Wróć na swoją główną gałąź (jeśli używasz 'master' zamiast 'main', zmień to poniżej)
git checkout eq

rm index.*

# 7. Usuń gałąź 'web-build' z dysku lokalnego.
# Dzięki temu przy następnym uruchomieniu skryptu krok 2 znów zadziała poprawnie.
git branch -D web-build
