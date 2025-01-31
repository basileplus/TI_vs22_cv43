
![](documents\attachments\documents\attachments\CR_TP-20250108161737976.png)

# Exercice 1

L'image initiale est 

![](documents\attachments\CR_TP-20250108161921113.png)

On commence par modifier le code pour afficher l'histogramme 1D
![](documents\attachments\CR_TP-20250108161509290.png)

Ensuite on modifie le fichier de config de l'exo1 en ``.cfg``. On peut modifier l'espace des couleurs dans ce fichier en changeant par exemple ``'RGB'`` par ``'YUV'``
**Capture images modifiées avec l'espace couleur**
### *Qu'est ce que l'espace HSV ?*
L'espace **HSV** (Hue, Saturation, Value) est un modèle de représentation des couleurs basé sur la perception humaine.

![](documents\attachments\CR_TP-20250108161904627.png)

- **H (Hue)** : Teinte, indique la couleur (0° = rouge, 120° = vert, 240° = bleu).
- **S (Saturation)** : Saturation, représente l'intensité ou la pureté de la couleur (0 = gris, 1 = couleur vive).
- **V (Value)** : Valeur, indique la luminosité (0 = noir, 1 = couleur maximale).

![](documents\attachments\CR_TP-20250108161737976.png)

Il est souvent utilisé en traitement d'image car il facilite la séparation de la couleur (teinte) et de l'intensité lumineuse.

## *Qu'est ce que l'espace Lab ?*

L'espace **Lab** (CIE 1976) est un modèle de couleur basé sur la perception humaine, conçu pour être indépendant des dispositifs de reproduction.

- **L (Lightness)** : Luminosité, allant de 0 (noir) à 100 (blanc).
- **a** : Composant allant de vert (-) à rouge (+).
- **b** : Composant allant de bleu (-) à jaune (+).

![](documents\attachments\CR_TP-20250108161821742.png)

Il est utilisé en traitement d'image car il sépare la luminance (L) et la chrominance (a, b), ce qui le rend plus adapté à des opérations de manipulation des couleurs.

# Exercice 2 : Modification de l'histogramme

## Etirement de l'histogramme

```c++
Mat Color::HistoStretching(Mat It)
{
	Mat Jt=Mat::zeros(It.rows, It.cols, CV_8U);
    /*===============A COMPLETER =============================*/
	// Etire l'histogramme de l'image It
	double minVal, maxVal;
	minMaxLoc(It, &minVal, &maxVal);
	It.convertTo(Jt, (CV_8U), 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

    /*===============FIN A COMPLETER =============================*/

	return(Jt);
}
```

Pour les images bruitées ou contenant peu d'information (variétés de couleurs), les modifications d’histogrammes risquent d’introduire du bruit en créant des variations artificielles de niveaux de gris.

![](documents\attachments\CR_TP-20250108163345175.png)

## Egalisation d'histogramme

![](documents\attachments\CR_TP-20250108163508349.png)

L'égalisation d'histogramme vise à obtenir un histogramme où toutes les valeurs de gris sont equiprobables. L'image semble surexposée avec cette modification de l'histogramme. Toutefois on est moins sensible au bruit qu'avec un étirement.
On vise un histogramme cumulé qui ressemble à une droite avec cette méthods
## Transformations log ou exp

Avec ces transformations, plutôt que de viser un histogramme cumulé linéaire nous allons viser un histogramme cumulé de forme logarithmique ou exponentielle. Ainsi augmente le contraste pour les hauts niveau de gris ou les faibles niveaux de gris.

![](documents\attachments\CR_TP-20250108164957993.png)

# Exercice 3 : représentation fréquentielle

On peut définir la transformée de Fourier d'une image. 

![](documents\attachments\CR_TP-20250108165327762.png)

Ainsi les axes u et v correspondent aux variables fréquentielles et l’origine (0, 0) (composante continue) est placée au centre de cette représentation. L’axe horizontal u (resp. vertical v) est associé aux variations d’intensité lumineuse dans l’image le long des lignes (resp. des colonnes).

![](documents\attachments\CR_TP-20250108165416835.png)

on constate que la composante continue (en (0,0)) est plus forte que les autres. On peut aussi remarquer que toutes les fréquences semblent présentent dans l'image (pas de zone noire). A partir de cette transformée de Fourier il est possible de filtrer l'image pour ne garder que les composantes basses fréquences ou hautes fréquences

![](documents\attachments\CR_TP-20250108165610423.png)

On peut jouer sur la fréquence de coupure de ces filtres numériques pour laisser plus ou moins de hautes ou basses fréquences.

# Exercice 5 : Clustering

Le clustering permet de former $K$ clusters de couleurs minimisant la variance intraclasse des pixels associés au cluster et maximisant la variance interclasse.
Le pseudo-code de l'algorithme est :

```pseudocode
Entrée: Image I (matrice de pixels), nombre de clusters K
Sortie: Image segmentée par K-means

1. Initialisation:
   - Choisir K centres de clusters initiaux aléatoires parmi les pixels de l'image I.
   - Chaque centre représente une couleur (ou un vecteur de caractéristiques, par exemple RGB).

2. Répéter jusqu'à convergence :
   
   2.1. Attribution des pixels aux clusters :
        - Pour chaque pixel P de l'image I, calculer la distance (souvent Euclidienne) entre P et chaque centre de cluster.
        - Assigner le pixel P au cluster dont le centre est le plus proche (la distance est la plus petite).

   2.2. Mise à jour des centres de clusters :
        - Pour chaque cluster, recalculer le centre en prenant la moyenne des pixels qui lui ont été assignés. Cela donne un nouveau centre de cluster.

3. Retourner l'image segmentée :
   - Chaque pixel de l'image est colorié avec la couleur du centre de son cluster associé.
```

Dans l'exemple suivant on se concentre sur le channel rouge afin de détecter le panneau de signalisation.

Si on travail dans l'espace de couleur ``RGB`` on obtient le clustering suivant :

![](documents\attachments\CR_TP-20250108172314211.png)

La méthode peine à détecter le panneau. Lorsque la configuration indique "RGB" k'ilage est gardée telle quel. Lorsqu'on modifie le fichier de configuration pour travailler dans l'espace "rgb" on normalise l'intensité de chaque composante rouge, vert, bleu par la luminance (somme des intensité rouge, verte, bleue). Ce choix possède à la fois des qualités et des défauts.

>[!important] Avec l'espace "rgb" chaque pixel va subir la normalisation
>```python
> R[i] = R[i]/(R[i]+G[i]+B[i])
> B[i] = B[i]/(R[i]+G[i]+B[i])
> G[i] = G[i]/(R[i]+G[i]+B[i])
> ```


- Pour segmenter un objet dont la couleur dénote sur l'image, alors l'espace "rgb" est à privilégier. Il permettra de rendre uniforme toutes les nuances de rouges du panneau. En revanche on perd des détails sur les ombres et les contour des objets. Si la on cherche à faire ressortir les couleur vives cet espace couleur est à privilégier. En revanche si on veut identifier les contours d'un objet dont la couleur n'est pas très différente du fond, alors travailler en RGB peut être plus intéressant.


![](documents\attachments\CR_TP-20250108171624370.png)

On fait un clustering sur 3 classes (3 niveaux de gris sur l'image la plus à droite) en regardant un seul canal (dans notre cas le canal rouge). On peut aussi modifier les paramètres pour ne faire le clustering que sur le canal vert, ou bleu. Les résultats sont les suivants :

![](documents\attachments\CR_TP-20250121141515434.png)
Canal vert
![](documents\attachments\CR_TP-20250121141549184.png)
Canal bleu

On constate que le clustering est plus efficace pour détecter le panneau rouge lorsque le clustering est effecuté sur la couleur rouge pour détecter le panneau rouge.

On peut aussi faire varier le nombre de classes $K$
![](documents\attachments\CR_TP-20250121141920093.png)
K = 2
![](documents\attachments\CR_TP-20250121141947477.png)
K=5

# Exercice 6 

On complète les confctions pour érroder et dilater les images* 
```python
Mat Morpho::erode(const Mat& src, const Mat& es, int N)
{
    int h= es.rows;
    int w= es.cols;
    int c= w/2;
    int r= h/2;
    Mat tmp=Mat::zeros(src.rows, src.cols, CV_8U);
    Mat dst=Mat::zeros(src.rows, src.cols, CV_8U);
    src.copyTo(tmp);
    for(int k=0; k<N; k++)
   {
    /*===============A COMPLETER =============================*/
        for (int i = r; i < src.rows - r; i++) // Parcours des pixels de l'image
        {
            for (int j = c; j < src.cols - c; j++)
            {
                bool erodePixel = true;
                // Parcourir l'élément structurant
                for (int y = -r; y <= r; y++)
                {
                    for (int x = -c; x <= c; x++)
                    {
                        // Vérifier si l'élément structurant est entièrement contenu
                        if (es.at<uchar>(y + r, x + c) != 0 &&
                            tmp.at<uchar>(i + y, j + x) == 0)
                        {
                            erodePixel = false;
                            break;
                        }
                    }
                    if (!erodePixel) break;
                }
                // Mettre à jour le pixel dans la matrice destination
                dst.at<uchar>(i, j) = erodePixel ? 255 : 0;
            }
        }
    /*===============FIN A COMPLETER =============================*/
        dst.copyTo(tmp);
        }
    return (dst);
}
```


![](documents\attachments\CR_TP-20250121151810754.png)

![](documents\attachments\CR_TP-20250121151830757.png)

On complète les fonctions suivantes :
- **`closing`** : Effectue une fermeture morphologique (dilatation suivie d'érosion) pour combler les petits trous et lisser les contours des objets clairs dans une image binaire.
- **`opening`** : Réalise une ouverture morphologique (érosion suivie de dilatation) pour supprimer les petits objets ou bruits tout en conservant la forme générale des objets plus grands.
- **`contours`** : Calcule les contours morphologiques en soustrayant l'image érodée de l'image originale, ce qui met en évidence les bordures des objets.
- **`reconstruction`** : Effectue une reconstruction géodésique, où une image "marqueur" est dilatée de manière itérative sous contrainte d'une image "masque", utile pour extraire des structures spécifiques.
- **`zeros`** : Détecte les passages par zéro dans l'image (changement de signe entre pixels adjacents), souvent utilisé pour localiser des contours ou des variations rapides.

On obtient les résultats suivants :
![](documents\attachments\CR_TP-20250121153010565.png)

La reconstruction géodésique permet de partir d'un image de marqueurs et de reconstruire les détails d'une image source

![](documents\attachments\CR_TP-20250121153919530.png)
(source : [Université de liège - Marc Van Droogenbroeck](https://www.telecom.ulg.ac.be/teaching/notes/totali/elen016/node126_tf.html))

# Exercice 7 : détection de contours

On recode les fonctions de convolutions nécessaires pour la détection de contour

![](documents\attachments\CR_TP-20250121173715238.png)

En particulier

```python
Mat Filtering::convolve(Mat I, Mat K)
{
	Mat J = Mat::zeros(I.rows, I.cols, CV_32F);

	int w = K.rows / 2;

	for (int i = w; i < I.rows - w; i++) {
		for (int j = w; j < I.cols - w; j++) {
			float res = 0.0;

			for (int u = -w; u <= w; u++) {
				for (int v = -w; v <= w; v++) {
					if (I.type() == CV_8U) {
						res += I.at<uchar>(i + u, j + v) *
							K.at<float>(w + u, w + v);
					}
					else if (I.type() == CV_32F) {
						res += I.at<float>(i + u, j + v) *
							K.at<float>(w + u, w + v);
					}
				}
			}
			J.at<float>(i, j) = res;
		}
	}

	return J;
}
```

La détection des contours par la norme du gradient repose sur le calcul des dérivées spatiales de l’image, généralement obtenues via des opérateurs comme Sobel. La norme du gradient, définie par \( $\sqrt{G_x^2 + G_y^2}$ \) (où \( $G_x$ \) et \( $G_y$ \) sont les dérivées horizontales et verticales), mesure l’intensité des variations de luminosité. Un seuillage fixe \($s$\) est ensuite appliqué pour identifier les contours : un seuil bas (\( $s = 10$ \)) capture des détails fins mais amplifie le bruit, tandis qu’un seuil élevé (\($s = 50$ \)) élimine les artefacts au détriment des contours subtils. Les limitations inhérentes à cette approche incluent sa sensibilité au bruit, car les gradients locaux peuvent être perturbés par des variations non significatives, ainsi que l’absence de suppression des non-maxima locaux, générant des contours épais. De plus, un seuil unique ne s’adapte pas aux variations locales de contraste, conduisant à des contours fragmentés dans les zones peu contrastées ou sous un éclairage hétérogène.

Le détecteur de Canny améliore cette méthode en introduisant une approche multi-étapes robuste. Il commence par lisser l’image avec un filtre gaussien d’écart-type \( $\sigma = s_b$ \), réduisant ainsi le bruit tout en préservant les structures. Les gradients horizontaux et verticaux sont calculés à l’aide d’opérateurs Sobel de taille \( $\text{apertureSize}$ \). La norme \( $L_2$ \) (activée par le paramètre \( $l_2$ \)) offre une précision accrue en utilisant \( $\sqrt{G_x^2 + G_y^2}$ \), contrairement à la norme \( $L_1$ \) (\( $|G_x| + |G_y|$ \)), moins coûteuse mais moins exacte. L’hystérésis des seuils, pilotée par \( $s_h$ \) (seuil haut) et un seuil bas typiquement fixé à \( $0.3 \times s_h$ \), permet de relier les contours forts et faibles tout en éliminant les réponses parasites :
- Si le gradient est supérieurs à $s_{h}$ alors le pixel est un contour
- Si le gradient est inférieur à $s_{b}$ ce n'est pas un contour
- Si le gradient est entre $s_{b}$ et $s_{h}$ alors c'est un contour si un de ses pixels voisins est un contour (hystérésis)

La détection des passages par zéro du Laplacien, quant à elle, repose sur les dérivées secondes de l’image, préalablement lissée par un filtre gaussien de largeur \( $W$ \), où \( $\sigma = W / 2$\). Le Laplacien, noté \($\nabla^2 I$ \), est calculé après convolution avec la gaussienne, et les contours correspondent aux points où la dérivée seconde s’annule (changements de signe). Une faible valeur de \( $W$ \) (par exemple \( $W = 2$ \), donc \( $\sigma = 1$ \)) préserve les détails fins mais accentue la sensibilité au bruit. À l’inverse, un \( $W$ \) élevé (\( $W = 10$ \), \( $\sigma = 5$ \)) lisse davantage l’image, produisant des contours plus larges et moins sensibles aux perturbations, au prix d’une perte de résolution spatiale. Cette méthode permet une analyse multi-échelle, mais elle reste globalement plus vulnérable au bruit que l’approche de Canny, notamment en présence de textures complexes.

# Exercice 8 : étiquetage en composantes connexes

Etiquetage : on part d'une image binaire -> on associe à chaque pixel de l'image une étiquette correspondant à la composante connexe à laquelle appartient le pixel
Au préalable on va passer l'image de départ dans un algorihme k-means qui va nous donner k cluster de couleurs, mais 2 pixels du même cluster peuvent ne pas être connectés ensemble, ils appartiendront donc à deux composantes connexes différentes 
On part d'un pixel "germe" de valeur 1. On va ajouter à la pile de pixels à traiter tous ses voisins de valeur 1 (tous les pixels appartenant à la même composante connexe), puis on va traiter tous ses voisins puis les voisins des voisins etc. Jusqu'à ce qu'on ait traiter tous les pixels de la composante connexe. Une fois la pile vide, on change de pixel jusqu'à tomber sur un nouveau pixel de valeur 1 à qui sera attribué une nouvelle composante connexe.

![[CR_TP 2025-01-30 14.25.52.excalidraw.png]]
%%[[CR_TP 2025-01-30 14.25.52.excalidraw.md|🖋 Edit in Excalidraw]]%%

On obtient les résultats suivants :

![](documents\attachments\CR_TP-20250130143600735.png)
![](documents\attachments\CR_TP-20250130143625443.png)
On observe sur cette image que certains pixels de bruit sont restés sur l'image binaire, ainsi on a détecté pour chacun d'eux une nouvelle composante connexe. Il faut donc faire attention à ce que l'image binaire ne soit pas bruité, on pourrait par exemple la passer à travers un filtre gaussien. On pourrait aussi appliquer un seuil sur l'image segmentée en imposant que chaque composante connexe doit avoir un nombre de pixel $N_{i}>\text{seuil}$ (pour éliminer les pixels isolés)

# Exercice 9

![](documents\attachments\CR_TP-20250130151244649.png)

![](documents\attachments\CR_TP-20250130151322666.png)

![](documents\attachments\CR_TP-20250130151407873.png)


### **Détection des cercles et des formes géométriques : Méthodes et limitations**

#### **1. Utilisation de la signature du contour pour détecter les cercles**

La signature du contour, qui représente la distance entre chaque point du contour et le centre d’inertie, est un indicateur efficace pour identifier les cercles. Pour une forme parfaitement circulaire, cette distance reste constante, entraînant une faible variance ou un écart-type proche de zéro. Une autre approche consiste à analyser son spectre fréquentiel via une transformée de Fourier : un cercle parfait ne présente aucune variation périodique et ne contient qu’une composante continue.

#### **2. Limites du rapport des moments centraux d’ordre 2**

L’utilisation du rapport $\frac{\mu_{02}}{\mu_{20}}​​$ permet d’analyser la symétrie d’une forme, mais cette mesure présente des limitations. Un rapport égal à 1 peut correspondre à un cercle, mais également à des formes symétriques non circulaires, comme un carré orienté à 45°. De plus, ce rapport est sensible à la rotation et peut être altéré par le bruit ou une segmentation imparfaite. Ainsi on détecte mal un panneau qui ne serait pas parfaitement en face de la caméra

#### **3. Exploitation de la compacité pour l’identification des cercles**

La compacité, définie par $\frac{4\pi A}{P^2}​$, est une mesure pertinente puisqu’elle vaut exactement 1 pour un cercle parfait. Cependant, cette valeur diminue pour les polygones réguliers :

- Triangle équilatéral : environ **0.604**
- Hexagone : environ **0.906**

Un seuil supérieur à **0.95** peut ainsi être utilisé pour distinguer les cercles des autres formes. Toutefois, cette méthode reste sensible aux contours irréguliers, aux formes partiellement occultées et aux déformations dues au bruit.

#### **4. Identification des formes géométriques par analyse de la signature**

L’analyse de la signature du contour permet de reconnaître différentes formes en fonction du nombre de pics et de creux observés :

- **Cercle** : signature relativement plate avec une faible variance.
- **Triangle** : trois pics correspondant aux sommets et trois creux pour les côtés.
- **Hexagone / Octogone** : six ou huit pics réguliers.

Deux approches peuvent être utilisées :

- Le comptage des pics dans la signature $D(t)$.
- L’analyse de la périodicité via une transformée de Fourier, qui permet par exemple d’identifier un triangle grâce à trois fréquences dominantes.

Toutefois cette méthode ne permet pas de détecter les panneaux lorsque plusieurs formes sont présentes, comme c'est le cas pour les photos suivantes :

![](documents\attachments\CR_TP-20250130151816726.png)

![](documents\attachments\CR_TP-20250130152152679.png)
#### **5. Problèmes rencontrés dans l’analyse du contour**

Certaines méthodes ne fonctionnent pas systématiquement en raison de plusieurs facteurs :

- **Seuil de compacité trop strict** : risque d’ignorer des cercles légèrement déformés.
- **Sensibilité au bruit** : apparition de faux pics ou creux dans la signature.
- **Dépendance à une segmentation parfaite** : une mauvaise détection des pixels rouges peut fausser les résultats.
- **Non-prise en compte de la rotation** : un triangle orienté différemment peut être mal identifié.

#### **6. Utilisation de la transformée de Hough pour la reconnaissance des formes**

La transformée de Hough est une alternative efficace pour détecter les formes géométriques :

- **Pour les cercles** : la transformée de Hough circulaire permet de repérer le centre et le rayon d’un cercle.
- **Pour les polygones** : la transformée de Hough linéaire permet d’identifier les segments qui composent une forme et de les regrouper en fonction de leur nombre et de leurs angles. Par exemple, trois segments indiqueront un triangle, tandis que six ou huit segments correspondront à un hexagone ou un octogone.

Cette approche est robuste face au bruit et aux formes partiellement occultées, mais elle reste coûteuse en termes de calcul, en particulier pour les images de grande taille.

Nous allons implémenter une méthode de comptage des cercles, traingles et hexagone en exploitant la signature de chaque forme :
- Si on détecte 0 maximum, alors la forme est un cercle
- Si on détecte 3 maximum, alors la forme est un triangle
- Si on détecte 6 maximums, alors la forme est un héxagone
Afin d'être robuste face au bruit, nous allons au préalable filtrer le signal de signature afin de retirer le bruit

On modifie la fonction ``analysis`` afin d'analyser les données précédentes et détecter les formes présentes. Une première version de ce code a été testé sans la moyenne glissante sur la signature. On s'aperçoit rapidement en débuggant que lalgorithme détecte beaucoup trop de pics dans la signature, ainsi on commence par lisser la signature afin de la rendre moins sensible au bruit et de ne pas détecter de pics là où il n'y en a pas. 

Les différentes méthodes décrites précédemment sont utilisées afin de croiser les méthodes et d'obtenir une détection la plus juste possible

```c++
#define N_MIN 50 // Seuil de taille minimale pour considérer une région
#define SEUIL_VAR 2.0 // Seuil de variance pour identifier un cercle
#define SEUIL_PEAKS_TRI_MIN 3 // Seuil minimum de pics pour un triangle
#define SEUIL_PEAKS_TRI_MAX 5 // Seuil maximum de pics pour un triangle
#define SEUIL_PEAKS_HEXA_MIN 5 // Seuil minimum de pics pour un hexagone
#define SEUIL_PEAKS_HEXA_MAX 8 // Seuil maximum de pics pour un hexagone
#define WINDOW_SIZE 5 // Taille de la fenêtre pour la moyenne glissante
void Shape::analysis()
{
    int nrond = 0, ntri = 0, nocto = 0;

    auto it = t_sig.begin();
    for (size_t k = 0; k < t_sig.size(); ++k, ++it) {
        int region_number = k + 1; // Les régions sont indexées à partir de 1

        // Ignorer les régions trop petites
        if (t_area[region_number] < N_MIN)
            continue;

        const auto& signature = *it;
        if (signature.empty())
            continue;

        // Calcul de la moyenne et variance de la signature
        double sum = 0.0;
        for (int d : signature)
            sum += d;
        double moy = sum / signature.size();

        double var = 0.0;
        for (int d : signature)
            var += (d - moy) * (d - moy);
        var /= signature.size();

        // Compacité (Périmètre² / (4 * π * Aire))
        double compacite = (t_perim[region_number] * t_perim[region_number]) / (4 * CV_PI * t_area[region_number]);

        // Détection des pics dans la signature avec moyenne glissante
        int peaks = 0;
        std::vector<double> smoothed_signature(signature.size(), 0.0);

        // Calcul de la moyenne glissante
        for (size_t i = 0; i < signature.size(); ++i) {
            int start = std::max(static_cast<int>(i) - WINDOW_SIZE / 2, 0);
            int end = std::min(static_cast<int>(i) + WINDOW_SIZE / 2, static_cast<int>(signature.size()) - 1);
            double sum = 0.0;
            for (int j = start; j <= end; ++j) {
                sum += signature[j];
            }
            smoothed_signature[i] = sum / (end - start + 1);
        }

        // Détection des pics dans la signature lissée
        for (size_t i = 1; i < smoothed_signature.size() - 1; ++i) {
            if (smoothed_signature[i] > smoothed_signature[i - 1] && smoothed_signature[i] > smoothed_signature[i + 1]) {
                peaks++;
            }
        }

        // Classification des formes
        if (var < SEUIL_VAR) {
            // Cercle : variance faible et compacité proche de 1
            nrond++;
        }
        else if (peaks >= SEUIL_PEAKS_TRI_MIN && peaks <= SEUIL_PEAKS_TRI_MAX ) {
            // Triangle : 3 à 5 pics et compacité élevée
            ntri++;
        }
        else if (peaks >= SEUIL_PEAKS_HEXA_MIN && peaks <= SEUIL_PEAKS_HEXA_MAX) {
            // Hexagone : 5 à 8 pics et compacité modérée
            nocto++;
        }
    }

    cout << "---------------------" << endl;
    cout << "CERCLES : " << nrond << endl;
    cout << "TRIANGLES : " << ntri << endl;
    cout << "HEXAGONES : " << nocto << endl;
    cout << "---------------------" << endl;
}
```

L'algorithme proposé parvient maintenant bien à détecter les formes présentes dans l'image, après un bon réglage des paramètres. Pour les images complexes comme la suivante, on a toujours du mal à obtenir le bon nombre de formes 

![](documents\attachments\CR_TP-20250130152152679.png)

# Exercice 10

On obtient les images suivantes. On commence ar judicieusement choisir le bon espace couleur pour détecter les toits en tuile.

![](documents\attachments\CR_TP-20250130174548649.png)
RGB

On réutilise le code de l'exercice 1 pour analyser les couleurs des images de toit.

![](documents\attachments\CR_TP-20250130175604910.png)

On constate que le channel 2 de l'espace Lab semble particulièrement pertinent pour identifier les toits. On observe maintenant les distributions de l'histogramme pour essayer de définir un seuillage de l'histogramme qui permette d'isoler les toits.

![](documents\attachments\CR_TP-20250130175920839.png)

Nous allons donc travailler sur le channel 2 de l'espace Lab pour la détection de batiments. Maintenant que nous avons identifier l'espace de couleur pertinent ainsi que le channel à considérer, nous allons pouvoir modifier l'histogramme, comme dans l'exercice 2 afin d'isoler les composantes de couleurs qui nous intéressent

![](documents\attachments\CR_TP-20250131082119401.png)
![](documents\attachments\CR_TP-20250131082237400.png)
Un simple étirement d'histogramme semble suffisant, une transformation plus complexe ne semble pas êre bénéfique pour notre étude. Ainsi nous allons effectuer un étirement d'histogramme sur l'image à traiter

La méthode fonctionne très bien pour la plupart des images, toutefois on constate que pour certaines le résultat n'est pas bon du tout, par exemple pour l'image suivante
![](documents\attachments\CR_TP-20250131084123180.png)
Etalement + Clustering + Segmentation pour $K=2$
L'étirement d'histogramme semble bien aider à isoler les batiments, en revanche lors de la segmentation et du clustering l'analyse n'est pas bonne, alors même qu'elle fonctionne très bien pour d'autres images, par exemple
![](documents\attachments\CR_TP-20250131084259515.png)
Etalement + Clustering + Segmentation pour $K=2$

Le problème est le suivant : les pixels du sol sont trop foncés et sont détectés comme appartenant à la même classe que les toits lors du clustering. Jusqu'à maintenant le nombre de cluster était fixé à 2 et cela donnait un résultat convenable. En augmentant le nombre de clusters à 3 on va permettre à l'algorithme de définir une classe intermédiaire, entre le clair du sol et le foncé des toits.
![](documents\attachments\CR_TP-20250131084834784.png)

On se concentre maintenant sur une nouvelle difficulté observée sur l'image suivante :

![](documents\attachments\CR_TP-20250131085001374.png)

Entre les maisons on détecte des pixels de même intensité que les toits des maisons. Toutefois ces pixels sont plus fins et situés au milieu de zones claires (le sol). Une première approche peut donc être d'appliquer un flou gaussien. Cela permettra de "diluer" les pixels sombres mais situés au milieu de zones claires, et on renforcera par la même occasion la robustesse de l'algorithme au bruit de l'image. On constate que le flou aide grandement à réduire l'impacte des petites structures

![](documents\attachments\CR_TP-20250131085652802.png)
Après application d'un flou gaussien avec un kernel de taille $11 \times 11$

En revanche quelques éléments sont toujours apparents. Deux solutions peuvent aider à cela : il est possible d'augmenter encore la taille du noyau, au risque de ne plus détecter de batîments trop petits. Une autre façon d'améliorer la détection est d'appliquer le flou après égalisation d'histogramme et non avant comme c'est le cas pour la figure précédente. Ainsi le filtrage de l'image sera facilité puisque les différentes composantes seront plus isolées et plus facilement séparables

En augmentant l'intensité du flitrage on obtient un résultat satisfaisant :
![](documents\attachments\CR_TP-20250131090955348.png)

En excluant les classes qui ne contiennent pas assez de pixels on saura discerner les maisons du bruit/barrières. Maintenant un nouveau problème se pose du fait du filtrage : lorsque les batiments sont trop proches les uns des autres, le filtre encourage à ne détecter qu'un bâtiment au lieu de trois, comme c'est le cas sur la photo suivante
![](documents\attachments\CR_TP-20250131091247324.png)

Une solution est d'utiliser un filtrage qui conserve les contours, par exemple une méthode de diffusion anisotropique.

Finalement en assemblant toutes ces méthodes de traitement, on parvient à correctement détecter tous les bâtiments dont les toits sont en tuiles 

![](documents\attachments\CR_TP-20250131104023187.png)
![](documents\attachments\CR_TP-20250131104116314.png)
![](documents\attachments\CR_TP-20250131104232339.png)
On notera toutefois que la méthode utilisée fonctionne bien pour détecter les toits en tuiles, par exemple pour l'image suivante on ne détecte pas correctement les toits

![](documents\attachments\CR_TP-20250131104150538.png)