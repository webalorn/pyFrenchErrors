# pyFrenchErrors

pyfe est un projet écrit en C++ dont le but est de permettre de convertire les messages d'erreurs de pythons en des messages plus lisibles, précis et adaptés dans le contexte de programmes simples écrits en python, blockly ou scratch. Il gère les cas les plus courrants, et renvoie, dans le cas contraire, l'erreur par défaut de python. Il a pour but de servir aux débutants et aux plus jeunes pour faciliter leur apprentissage du langage.

## Quick start

### Installation

- Télécharger le code : `git clone <url du repo>`
- Compiler le projet `make`

L'executable est le fichier `pyfe`. Il peut être déplacé, mais il doit toujours y avoir le fichier `errorMessages.json` copié dans le même dossier.

### Utilisation

```bash
pyfe solution.py pythonStderr outputDest.json [target(=python)] [langage(=fr)]
```

- `solution.py` : chemin vers un fichier contenant le code python qui a été exécuté
- `pythonStderr` : chemin vers un fichier contenant *la sortie d'erreure* de l'interpéteur python uniquement (pas la sortie standard !).
- `outputDest.json` : chemin vers un fichier (éventuellement à créer) où sera écrit une description du résultat en JSON, *uniquement si pyfe se termine correctement*
- `target` : `python`, `blockly` ou `scratch`.

Le programme renvoie 0 si il s'est bien executé. Dans le cas contraire, ne pas utiliser la sortie standard ni le fichier `outputDest.json`. Il renvoie par exemple 1 si aucune erreur n'a été trouvée.

#### Sortie JSON

Si le programme s'est correctement éxécuté, le fichier `outputDest.json` sera écrit selon le format suivant :

```json
{
    "blockId": null,
    "causeFound": true, // Si le message a bien été interprété
    "line": [id_de_la_ligne], // Entier >= 1
    "messageId": "[identifiant_de_l_erreur]",
    "stderr": "[Sortie d'erreur de python]",
    "text": "[Message interprété par pyfe]",
    "vars": {...} // Variables utilisées dans le message
}
```

- Si `causeFound` vaut false, `messageId` sera vide, et `text` correspondra à la sortie d'erreur de python.

### Scripts utilitaires

- Tester en local sur un fichier python : `./testOn.sh path_to_code.py`
- Lancer les tests : `python3 tests/run.py`
    - La suite de tests n'est que partielle.

## Format du fichier JSON errorMessages.json

Ce fichier a pour but de regroupper les messages qui seront affichés, dans différentes langues. L'objet principal contient des clées, chacune correpondant à un langage. Le contenu de ses sous-objets est un ensemble de couples clé/valeur, la clé correpondant à un type d'erreur, la valeur au texte qui sera affiché.

- Pour associer plusieures clées à un seul texte, on peut les séparer pas `'|'` comme ici : `"typeError1|typeError2": "myMessage"`
- Un texte peut inclure des paramètres sous la forme: `{{param_name}}`, avec 'param_name' le nom du paramètre, tel que défini dans `errorParser.cpp`. La clé `{{target}}` est toujours définie, et vaut `python`, `blockly` ou `scratch` selon le cas.

### Exemple:

```json
{ "fr": {
    "python" : {
        "IndexError|KeyError": "Clée ou index invalide",
        "ZeroDivisionError": "Division par zéro, dans le langage {{target}}."
    }
}, "en":{
    "python" : {
        "IndexError|KeyError": "invalid key or index",
        "ZeroDivisionError": "Division by zero"
    }
}}
```

