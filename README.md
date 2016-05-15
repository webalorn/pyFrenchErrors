# pyFrenchErrors
Projet en c++ dont le but est de permettre une compréhension plus aisée des messages d'erreur python en fançais (ou autre langue). Il se base su le parsing des messages d'erreur de python et un parcour simple du code pour détecter les erreures les plus courantes et pouvoir, outre traduire les messages d'erreure, déterminer de manière plus précise que python ces causes. Il a pour but de servir aux débutants et aux plus jeunes pour faciliter leur apprentissage du langage.

## Format des fichiers JSON
#### errorMessageTranslate.json
Ce fichier a pour but de regroupper les messages qui seront affichés, dans différentes langues. L'objet principal contient des clées, chacune correpondant à un langage. Le contenu de ses sous-objets est un ensemble de couples clé/valeur, la clé correpondant à un type d'erreure, la valeur au texte qui sera affiché.
- Pour aassocier plusieures clée à un seul texte, on peut les séparer pas `'|'` comme ici: `"typeError1|typeError2": "myMessage"`
- Un texte peut inclures des paramètres (décrits dans `pyErrorMeaningTree.json`), sous la forme: `{{6}}`, avec en '6' le numéro du paramètre (le premier correpond à `{{0}}`).
###### Exemple:
```
{ "fr": {
    "indexError|KeyError": "Clée ou index invalide",
    "ZeroDivisionError": "Division pas zéro"
}, "en":{
    "indexError|KeyError": "invalid key or index",
    "ZeroDivisionError": "Division by zero"
}}
```
#### pyErrorMeaningTree.json
Ce fichier décrit l'arbre de choix qui va déterminer, pour une erreure, quel message(s) à afficher, en se basant sur des regex et des appels à des fonctions c++ (fichier meaningTreeFcts.cpp). 

##### Les différents types d'objets:
###### Bloc regex
- Un attribut **"type"** avec comme valeur:
    - **"errorMessage"** (regex sur le message d'erreur python)
    - **"errorType"** (regex sur le type d'erreur/exception donné par python, souvent simple vérification d'égalité)
    - **"errorCodeLine"** (regex sur la ligne d'erreure donnée par python
- OU ALORS: au lieu de la clée **"type"**, une clée **"regexOn"**, avec en valeur une `parsableValue` (cf plus bas). La regex se fera sur la chaine obtenue en parsant cette valeur. 
- *[OPTIONEL]* attribut **"var"**: nom de la variable dans laquelle le résultat de la regex sera stockée (cf plus bas, parsableValue)
- Un ensemble de paires clée/valeur: la clée représente la regex, la valeur est un bloc qui sera éxécuté si la regex match totalement la chaine évaluée (**"type"** ou **"regexOn"**). L'ordre du fichier n'est pas celui de l'évaluation (l'ordre ne doit pas importer)
- *[OPTIONEL]* un attribut **"default"** évalué si a la fin du processus de traitemenr de l'objet aucun résultat n'a été trouvé. (Toutes les regex sont fausses ou leur bloc renvoie un ensemble vide)

###### Bloc conditionel
- Un attribut **"condition"**, avec en valeur le nom d'une fonction booléenne (depuis getBoolTreeFcts() dans meaningTreeFcts.cpp). Le retour de la fonction déterminera la valeur de la condition
- *[OPTIONEL]* Un attribut **"params"**: un array de `parsableValue` qui seront évaluée et passée en paramètre à la fonction.
- *[OPTIONEL]* Un attribut **"block"**: l'objet qui sera évalué si la condition est vraie
- *[OPTIONEL]* Un attrubut **"else"**: l'objet qui sera évalué si la condition est fausse.

###### Bloc typeError
- Un attribut **"typeError"** dont la valeur sera retournée comme nom d'erreure principale du bloc
- *[OPTIONEL]* attribut **"realErrorLine"** : une `parsableValue` qui sera parsée en int pour préciser la vraie ligne de l'erreure si celle fournie par python est fausse.
- *[OPTIONEL]* Attribut **"concatenate"**: [{...}, {...}] Un array d'objets. Les types d'erreurs principaux et secondaires retournés par chaque bloc seront ajouté comme type d'erreur secondaires. Les messages secondaires seront concaténé avec le premier à l'affichage, pour donner des infos supplémentaires.
- *[OPTIONEL]* Attribut **"params"**: paramètres passés à errorMessageTranslate.json pour remplacer les {{?}} dans les chaines.

Cette écriture étant lourde, si une chaine de caractère est rencontrée en lieu et place d'un bloc (ex: *"monTypeErreur"*), elle sera évaluée comme un bloc typeError (ici: {**"typeError"**: *"monTypeErreur"*})

###### Array d'objets
Si un array d'objets est rencontré en lieu et place d'un objet, alors ses valeures seront évaluées dans l'ordre une par une, en s'arrétant dès qu'un type d'erreur est trouvé.
ex: {**"regexOn"**: *"txt[1]"*, ".*": [{...}, {...}]}

###### attributs généraux
Tous les objets peuvent avoir les attribut suivants:
- **"pyVmin"**: Si la version de python utilisée est strictement inférieure, alors le bloc n'est pas évalué. Un version de python est juste une suite de chiffres séparés par des points.
- **"pyVmax"**: Si la version de python utilisée est strictement supérieure, alors le bloc n'est pas évalué

###### Les parsableValues
Un parsable value est une chaine quelquonque, qui, si elle répond à un format spécifiquem sera évaluée selon un protocole sécifié ci-dessous.
- Chaine de la forme **"nom d'une fonction(1 paramètre)"**: la chaine sera évaluée comme la valeur de retour de la fonction *\<nom d'une fonction\>*, en lui passant en paramètre un string qui sera *\<1 paramètre\>* (éventuellement "").
- Chaine de la forme **"nom variable[X]"**: sera évalué comme la valeur du **Xem** paramètre extrait de la regex ayant été stocké avec **"var"**: "*\<nom variable\>*".
- Si **aucun** des deux cas, renvoie la valeur qui a étée parsée. (*"hgl"* retourne *"hlg"*)

## Utilisation des modules
Le fichier **main.cpp** est ici présent à titre d'exemple: c'est lui qui doit être modifié pour adapter le code à votre utilisation.
Vous aurez probablement besoin des includes suivants:
```
#include "pyError.hpp"
#include "pyErrorMeaningTree.hpp"
#include "tradErrorMessages.hpp"
#include "pyFile.hpp"
```
#### Objets persistants
###### Objet *PyErrorMeaningTree*
Il doit être construit en lui passant un flux sur le fichier **pyErrorMeaningTree.json** . Il peut être conservé ensuite au fil des requètes (une seule construction pour tester de mutliples erreures). Il a pour but de déterminer le(s) id du/des message(s) à afficher.

###### Object *TradErrorMessages*
Il doit être construit en lui passant un flux sur le fichier **errorMessageTranslate.json** . Il peut être conservé ensuite au fil des requètes (une seule construction pour tester de mutliples erreures). Il transformera un ensemble d'ID de messages d'erreur en un message intelligible dans la langue cible.

#### Objets temporaires
###### Objet *PyFile*
Il doit être construit en lui passant en paramètre un std::vector\<std::string\> des lignes composant le fichier de code python qui a déclenché l'erreure.
###### Objet *PyError*
Il doit être construit en lui passant en paramètre un std::vector\<std::string\> des lignes de la sortie **d'erreur uniquement** de python.

#### Fonctions pour intepréter le message
###### \<PyErrorMeaningTree\>.*getMeaningMessages*(\<PyError\>, \<PyFile\>, version de python)
Va retourner un objet de type **errorDescription** qui contient les id des messages d'erreur, et le vrai numéro de la ligne d'erreur.
La ligne de l'erreur est acceccible via: \<errorDescription\>.errLine

###### \<TradErrorMessages\>.getMessage(\<errorDescription\>.messages, langage)
Va retourner le message correpondant aux messages passés en paramètres, concaténés (avec un " " entre eux), dans la langue "langage". La chaine peut être vide.
