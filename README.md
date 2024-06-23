# opengl-app
## Projet computer graphic
### Author : Xiumin LIN & Romain LOYER

Info:
- OpenGL 2.1
- C++14
- OS: macOS

Requirement:
- GLEW
- GLFW
Le dossier `assets` contient des fichiers .OBJ de tests.

# Paramètres de Configuration pour Affichage d'Objets 3D

## Paramètres pour `teapot.obj`
- **Camera:**
  - Rayon initial : 40 (configuré dans `main.cpp`)
  - Sensibilité du scroll : 1 (configuré dans `CameraOrbital.cpp`)
- **Matrice du monde:**
  - Échelle : 1.0, 1.0, 1.0 (configuré dans `Application.cpp`)
- **Textures:**
  - Utilisation de `brick.png` et `brick-specular.png` configurées dans le fichier MTL.

## Paramètres pour `yoda.obj`
- **Camera:**
  - Rayon initial : 50 (configuré dans `main.cpp`)
  - Sensibilité du scroll : 5 (configuré dans `CameraOrbital.cpp`)
- **Matrice du monde:**
  - Échelle : 0.01f, 1.01f, 1.01f (configuré dans `Application.cpp`)
- **Textures et matériaux:**
  - Via le fichier yoda.mtl

## Paramètres pour `cube.obj`
- **Camera:**
  - Rayon initial : 4 (configuré dans `main.cpp`)
  - Sensibilité du scroll : 1 (configuré dans `CameraOrbital.cpp`)
- **Matrice du monde:**
  - Échelle : 1.0, 1.0, 1.0 (configuré dans `Application.cpp`)
- **Textures:**
  - Pas de texture


# Guide des Contrôles de Caméra et de Déplacement

## Contrôles de la souris
- **Caméra orbitale:** Utilisez la souris pour contrôler l'angle de vue autour de l'objet.

## Contrôles du clavier
- **Déplacement (WASD):**
  - **W:** Avancer
  - **A:** Déplacer à gauche
  - **S:** Reculer
  - **D:** Déplacer à droite
- **Fonctions supplémentaires:**
  - **Échappe:** Fermer la fenêtre
  - **F:** Focaliser sur une mèche spécifique, utile lorsque plusieurs mèches sont présentes.
  - **U:** Désactiver le focus, permettant à nouveau l'utilisation des touches WASD pour le déplacement standard.
