#include <sil/sil.hpp>
#include "random.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

void seulementLeVert(sil::Image image) 
{
    for (glm::vec3 & color : image.pixels()) 
    {
        color.r = 0.f /*mets la composante rouge à 0. 1 couleur = glm::vec3 */ ;
        color.b = 0.f;
    };
    
    image.save("output/01_seulementLeVert.png");
};

void echangeRougeBleu(sil::Image image) 
{
    for (glm::vec3 & color : image.pixels()) 
    {
        std::swap(color.r, color.b);
    };

    image.save("output/02_echangeRougeBleu.png");
};

void noirEtBlanc(sil::Image image) 
{
    for (glm::vec3 & color : image.pixels()) 
    {
        float moy = (color.r + color.g + color.b) / 3;
        color.r = moy;
        color.g = moy;
        color.b = moy;
    };

    image.save("output/03_NoirEtBlanc.png");
};

void negatif(sil::Image image) 
{
    for (glm::vec3 & color : image.pixels()) 
    {
        color.r = 1 - color.r;
        color.g = 1 - color.g;
        color.b = 1 - color.b;
    };

    image.save("output/04_Negatif.png");
};

void degrade(sil::Image image) 
{
    for (int y{0}; y < image.height(); y++)
    {
        for (int x{0}; x < image.width(); x++)
        {

            image.pixel(x, y).r += x / (image.width() - 1.0);
            image.pixel(x, y).g += x / (image.width() - 1.0);
            image.pixel(x, y).b += x / (image.width() - 1.0);

            /*
            Autre façon de faire, en utilisant la valeur précédente en incrémentant :
            image.pixel(x, y).r += image.pixel(x-1, 0).r + 1.f/(image.width() -1);
            Attention : faire démarrer x à 1 et conv la division en float
            y = 0 car on s'en fiche de la ligne ici
            */
        };
    };

    image.save("output/05_degrade.png");
};

void miroirReverse (sil::Image image) 
{
    for (int i{0}; i < image.height()*image.width(); i+=image.width())
    {
        std::reverse(image.pixels().begin() + i, image.pixels().begin() + i + image.width()); 
    };

    image.save("output/06_miroirReverse.png");
};

void miroir2 (sil::Image image) /*2e version*/
{
    // On passe sur tous les x et tous les y, et on accède au pixel correspondant :
    for (float x{0}; x < image.width()/2.f; x++)
    {
        for (float y{0}; y < image.height(); y++)
        {   float tmp_r {image.pixel(x,y).r};
            float tmp_g {image.pixel(x,y).g};
            float tmp_b {image.pixel(x,y).b};

            //on peut utiliser swap(a,b) en pratique
            image.pixel(x,y).r = image.pixel(image.width()-(x+1),y).r;
            image.pixel(x,y).g = image.pixel(image.width()-(x+1),y).g;
            image.pixel(x,y).b = image.pixel(image.width()-(x+1),y).b;
            image.pixel(image.width()-(x+1),y).r = tmp_r;
            image.pixel(image.width()-(x+1),y).g = tmp_g;
            image.pixel(image.width()-(x+1),y).b = tmp_b;
        }
    }
    image.save("output/06_miroir2.png");
}

void bruit(sil::Image image) 
{
    int noiseLvl{0};

    std::cout << "Enter noise level" << std::endl;
    std::cin >> noiseLvl;
    
    for (int i{0}; i < noiseLvl; i++)
    {
        int x = random_int(0, image.width());
        int y = random_int(0, image.height());

        image.pixel(x, y).r = random_float(0, 1);
        image.pixel(x, y).g = random_float(0, 1);
        image.pixel(x, y).b = random_float(0, 1);
    };

    image.save("output/07_Noise.png");
};

void rotation90(sil::Image image) 
{
    sil::Image canvas{image.height(), image.width()};

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            canvas.pixel(image.height() - 1 - y, x) = image.pixel(x, y);
        };
    };

    canvas.save("output/08_90deg.png");
};

void RGBSplit(sil::Image image, sil::Image result)
{
    // On passe sur tous les x et tous les y, et on accède au pixel correspondant :
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if (x<30){  // pour gerer les premieres colonnes
                result.pixel(x,y).r = image.pixel(0,y).r;
            }else{
                result.pixel(x,y).r = image.pixel(x-30,y).r;
            }

            if (x>image.width()-31){  //pour gerer les dernieres colonnes
                result.pixel(x,y).b = image.pixel(299,y).b;
            }else{
                result.pixel(x,y).b = image.pixel(x+30,y).b;
            }
            result.pixel(x,y).g = image.pixel(x,y).g;
        }
    }
    result.save("output/09_RGBSplit.png");
}

int main()
{
    set_random_seed(0);
    sil::Image logo{"images/logo.png"};
    sil::Image lowContrast{"images/photo_faible_contraste.jpg"};
    sil::Image blackRectangle{300, 200};
    sil::Image result2{300, 345};   //meme taille que logo

    seulementLeVert(logo);
    echangeRougeBleu(logo);
    noirEtBlanc(logo);
    negatif(logo);
    degrade(blackRectangle);
    miroirReverse(logo); 
    miroir2(logo);
    bruit(logo);
    rotation90(logo);
    RGBSplit(logo, result2);
}