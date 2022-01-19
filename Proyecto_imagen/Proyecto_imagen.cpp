/* Proyecto Terminal
* Transformaciones geom�tricas de im�genes:
* traslaci�n, rotaci�n, escalamiento, transformaci�n affine
* Herramientas: OpenCV, C++
* Autor: Diego Delez� Su�rez
* Versi�n: 19
* Revisado y modificado: Dr. Carlos Joel Rivero Moreno
* Fecha (aaaa-mm-dd): 2022-01-14
*/

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include <array>

using namespace cv;
using namespace std;

Mat leer(String);
Mat trasladarOld(Mat);
Mat trasladar(Mat, int, int, int = cv::BORDER_REPLICATE);
Mat rotar(Mat, float, int = cv::BORDER_CONSTANT);
Mat rotar_ajuste(Mat, float, int, int, int = cv::BORDER_REPLICATE);
Mat escalar(Mat, int, int, int = cv::BORDER_REPLICATE);
Mat escalar_ajuste(Mat, int, int, int = cv::BORDER_REPLICATE);
Mat binarizar(Mat);
Mat escala_grises(Mat);
Mat recortar_imagen(Mat, int, int);
Mat unir_imagen(Mat,int);
void leeParametros(int&, int&, float&, int&, int&);
void reordenar_indices_imagen(std::array<int, 16>&);
void show_indexes(void);
//void ShowManyImages(const std::array, const, int);

const cv::String strWinName1 = "Imagen Original";
const cv::String strWinName2 = "Imagen Trasladada";
const cv::String strWinName3 = "Imagen Rotada";
const cv::String strWinName4 = "Imagen Escalada";
//const cv::String strWinName5 = "Imagen en niveles de gris";
const cv::String strWinName5 = "Imagen Biranizada";
const cv::String strWinName6 = "Imagen Cortada ";

const cv::String strWinName7 = "Recortada";
const cv::String strWinName8 = "rotar(angulo)";
const cv::String strWinName9 = "rotar(-angulo)";

/*
* cv::Mat image Copies (copias de im�genes cv::Mat)
- Shallow Copy (copia superficial):
The assignment operator and the copy constructor only copies the header.
- Deep Copy (copia profunda):
The underlying matrix of an image may be copied using the cv::Mat::clone() and cv::Mat::copyTo() functions.
*/

std::array<int, 16> orden_original = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
std::array <int, 16> orden_aleatorio = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

//int orden_original[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
//int orden_aleatorio[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

// Funci�n principal que procesa una imagen
int main(int argc, char** argv) {
    // Funci�n principal que procesa una imagen

    // Nombre de la imagen a color
    String strImageName = "imagen1.jpg";
    if (argc > 1) {
        strImageName = (String)argv[1];
    }

    // Lee la imagen strImageName y la traslada en la cantidad predefinida en trasladar()
    Mat imgOriginal = leer(strImageName);

    // Muestra las dimensiones de la imagen original3
    int current_height = imgOriginal.rows;
    int current_width = imgOriginal.cols;
    cout << "Tamano de imagen original: " << endl;
    cout << "ancho = " << current_width << ", alto = " << current_height << endl << endl;

    cout << "Tamano de cada sub-imagen (imagen original recortada): " << endl;
    cout << "ancho = " << current_width / 4 << ", alto = " << current_height / 4 << endl << endl;


    // Lee par�metros de transformaci�n (traslaci�n, rotaci�n, escala)
    int tx, ty; float angulo; int new_heigth, new_width;
    // tx = 200; ty = 200; angulo = 45;

    leeParametros(tx, ty, angulo, new_heigth, new_width);
    reordenar_indices_imagen(orden_aleatorio);
    // Procedimiento para obtener de �ndices(i, j) = (row, col) el �ndice k = num_img del arreglo unidimensional
        // obtener n�mero de imagen dado el rengl�n y columna

    int k, num_img, row, col, min = 1, max = 4;
    Mat imgGray;

    /*
    *  int i, j;
        for (i = 1; i < 4; i++) // renglones de subim�genes
    {
        for (j = 1; j < 4; j++) // columnas de subim�genes
        {
            k = 4 * (i - 1) + j - 1;
            num_img = orden_aleatorio[k];
            //obtener n�mero de rengl�n y de columna dado el n�mero de imagen
            col = ((num_img % 4) ? (num_img % 4) : 4);
            row = ((num_img - col) / 4) + 1;
            Mat imgGray = recortar_imagen(imgOriginal, row, col);
            // Falta la funci�n que va ensamblando las subim�genes en una imagen del tama�o de la original
        }
    }
    */

    // Muestra los indices (row,col) de la subimagen a partir de su indice num_img
    show_indexes();

    //for (k = 0; k < 16; k++) {
    k = 7;
        num_img = orden_aleatorio[k];
        //obtener n�mero de rengl�n y de columna dado el n�mero de imagen
        col = ((num_img % 4) ? (num_img % 4) : 4);
        row = ((num_img - col) / 4) + 1;
        imgGray = recortar_imagen(imgOriginal, row, col);
        /* Procedimiento para generar n�mero aleatorio de 1-4 para las transformaciones
         *  1- Rotar
         *  2- Trasladar
         *  3- Escalar
         *  4- Binarizar
       */
       
        srand(time(NULL));
        random_device indiceAletorio; //Variable aletoria
        mt19937 rango(indiceAletorio());// Algoritmo Mersenne twister: http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/ARTICLES/mt.pdf 
        uniform_int_distribution<> distribucion(min, max);//Distribuci�n Unifomre del 1 al 16
        //int transformacion = distribucion(rango);

        int transformacion; // TEST : Erase later
        cout << "(1 Rotar, 2 Trasladar, 3 Escalar, 4 Binarizar) transformacion = "; // TEST : Erase later
        cin >> transformacion; // TEST : Erase later
        //int transformacion = 3;
        cout << "El numero de transformacion es: " << transformacion << endl << endl;


        Mat imgRotated, imgTranslated, imgScaled, imgBinarized;

        //Mat imgTranslated = trasladar(imgOriginal, tx, ty);
        //Mat imgTranslated = trasladar(imgOriginal, tx, ty, BORDER_WRAP);
        //Mat imgTranslated = trasladar(imgGray, tx, ty, BORDER_REPLICATE);
        //Mat imgRotated = rotar(imgGray, angulo);
        //Mat imgScaled = escalar(imgGray, new_heigth, new_width);
        //Mat imgGrayscale = escala_grises(imgOriginal);
        //Mat imgBinarized = binarizar(imgGray);
       
        //trans_img = transformaciones[i];

        namedWindow(strWinName1, WINDOW_AUTOSIZE);
        imshow(strWinName1, imgOriginal);
        namedWindow(strWinName7, WINDOW_AUTOSIZE);
        imshow(strWinName7, imgGray);

        if (transformacion == 1) {
            //Mat imgRotated;
            float anguloResidual;
            anguloResidual = angulo - (float)((int)(angulo / 90.0f) * 90);
            anguloResidual = ((anguloResidual < 0) ? (-anguloResidual) : (anguloResidual));
            if (anguloResidual < 1.0e-6) {
                imgRotated = rotar(imgGray, angulo);
            }
            else {
                int width = imgGray.cols;
                int height = imgGray.rows;
                imgRotated = rotar(imgGray, angulo, cv::BORDER_REPLICATE);
                namedWindow(strWinName8, WINDOW_AUTOSIZE);
                imshow(strWinName8, imgRotated);
                imgRotated = rotar(imgRotated, -angulo, cv::BORDER_REPLICATE);
                namedWindow(strWinName9, WINDOW_AUTOSIZE);
                imshow(strWinName9, imgRotated);
                imgRotated = rotar_ajuste(imgRotated, angulo, width, height, cv::BORDER_REPLICATE);
            }
            namedWindow(strWinName3, WINDOW_AUTOSIZE);
            imshow(strWinName3, imgRotated);
        }
        else if (transformacion == 2) {
            //Mat imgTranslated;
            int borderMode;
            //borderMode = cv::BORDER_REPLICATE;
            borderMode = BORDER_REFLECT_101; // BORDER_DEFAULT;
            imgTranslated = trasladar(imgGray, tx, ty, borderMode);
            namedWindow(strWinName2, WINDOW_AUTOSIZE);
            imshow(strWinName2, imgTranslated);
        }
        else if (transformacion == 3) {
            //Mat imgScaled;
            int borderMode;
            borderMode = cv::BORDER_REPLICATE;
            //borderMode = BORDER_REFLECT_101; // BORDER_DEFAULT;

            //Mat imgScaled = escalar(imgGray, new_heigth, new_width, borderMode);

            imgScaled = escalar_ajuste(imgGray, new_heigth, new_width, borderMode);
            imgGray.copyTo(imgScaled);

            //if (!(((int)new_heigth == current_height) && ((int)new_width == current_width))) {
                //imgScaled = escalar_ajuste(imgGray, new_heigth, new_width, cv::BORDER_REPLICATE);
                //namedWindow(strWinName9, WINDOW_AUTOSIZE);
                //imshow(strWinName9, imgScaled);
            //}

            namedWindow(strWinName4, WINDOW_AUTOSIZE);
            imshow(strWinName4, imgScaled);
        }
        else if (transformacion == 4) {     
            //Mat imgBinarized;
            imgBinarized = binarizar(imgGray);
            namedWindow(strWinName5, WINDOW_AUTOSIZE);
            imshow(strWinName5, imgScaled);
        }

        //imwrite("imGray.jpg", imgGray);
        //imshow("imgGray", imgGray);
        
    //} // endfor
  
    //int  range = (max - min);
    //indiceAleatorio =  rand() % 16;
   
    if (false) {
        imgGray = recortar_imagen(imgOriginal, row, col);
        Mat imgTranslated = trasladar(imgGray, tx, ty, BORDER_REPLICATE);
        Mat imgRotated = rotar(imgGray, angulo);
        Mat imgScaled = escalar(imgGray, new_heigth, new_width);
        //Mat imgGrayscale = escala_grises(imgOriginal);
        Mat imgBiranizar = binarizar(imgGray);
        Mat img_unir = unir_imagen(imgGray, k);


        // Muestra im�genes en una misma ventana
        // A la izquierda, imagen original; a la derecha, imagen transformada
        /*
        ShowManyImages("Imagen original trasladada", 2, imgOriginal, imgTranslated);
        ShowManyImages("Imagen original rotada", 2, imgOriginal, imgRotated);
        ShowManyImages("Imagen original escalada", 2, imgOriginal, imgScaled);
        ShowManyImages("Imagen orginal en niveles de gris", 2, imgOriginal, imgGrayscale);
        */

        // Muestra imagen original 
        namedWindow(strWinName1, WINDOW_AUTOSIZE);
        imshow(strWinName1, imgOriginal);
        // Muestra imagen trasladada
        namedWindow(strWinName2, WINDOW_AUTOSIZE);
        imshow(strWinName2, imgTranslated);
        // Muestra imagen rotada
        namedWindow(strWinName3, WINDOW_AUTOSIZE);
        imshow(strWinName3, imgRotated);
        // Muestra imagen escalada
        namedWindow(strWinName4, WINDOW_AUTOSIZE);
        imshow(strWinName4, imgScaled);
        // Muestra imagen en biranizada
        namedWindow(strWinName5, WINDOW_AUTOSIZE);
        imshow(strWinName5, imgBiranizar);

        //Muestra imagen recortada
        namedWindow(strWinName6, WINDOW_AUTOSIZE);
        imshow(strWinName6, imgGray);
    } //  if (false)

    reordenar_indices_imagen(orden_aleatorio);
    
    
    waitKey();
    destroyAllWindows();

    // Regresa al sistema operativo
    return 0;
}

// Funci�n que lee par�metros de transformaci�n (traslaci�n, rotaci�n, escala)
void leeParametros(int& tx, int& ty, float& angulo, int& new_heigth, int& new_width) {
    // Funci�n que lee par�metros de transformaci�n (traslaci�n, rotaci�n, escala)

    cout << "Valores para trasladar la sub-imagen:" << endl << endl;

    cout << "Cantidad entera horizontal en pixels, x = ";
    cin >> tx;

    cout << "Cantidad entera vertical en pixels, y = ";
    cin >> ty;

    cout << endl;
    cout << "Valores para rotar la sub-imagen:" << endl << endl;

    cout << "Cantidad flotante en grados, angulo = ";
    cin >> angulo;

    cout << endl;

    cout << "Valores para escalar el tamano de la sub-imagen:" << endl << endl;

    cout << "Cantidad entera de ancho en pixels, ancho = ";
    cin >> new_width;

    cout << "Cantidad entera de alto en pixels, alto = ";
    cin >> new_heigth;

    cout << endl;

}

// Funci�n que lee una imagen
Mat leer(String strImageName) {
    // Funci�n que lee una imagen

    // Carga la imagen de color con el nombre especificado en strImageName
    Mat imgInput = imread(strImageName, CV_LOAD_IMAGE_COLOR);

    // Verifica si la imagen esta vacia, en cuyo caso imprime mensaje de error
    if (imgInput.empty()) {
        cout << "Error: the image has been incorrectly loaded." << endl;
    }

    // Regresa la imagen le�da
    return (imgInput);
}

// Funci�n que traslada una imagen en una cantidad eligida por el usuario
Mat trasladarOld(Mat imgOriginal) {
    // Funci�n que traslada una imagen en una cantidad eligida por el usuario

    // Cantidad que el usario va trasladar la imagen en coordena en x Y y
    int x;
    cout << "Coordenada en x: ";
    cin >> x;

    int y;
    cout << "Coordenada en y: ";
    cin >> y;

    int width = imgOriginal.cols - x;
    int height = imgOriginal.rows - y;

    // Llama al constructor normal y crea imagen similar a la original en negro (ceros)
    Mat imgTranslated(imgOriginal.size(), imgOriginal.type(), cv::Scalar::all(0));

    // Traslada en cantidad predefinida copiando rect�ngulo de imagen original a imagen trasladada
    //imgOriginal(Rect(x, y, width, height)).copyTo(imgTranslated(Rect(x-x, y-y, width-x, height-y)));
    imgOriginal(Rect(x, y, width, height)).copyTo(imgTranslated);

    // Regresa la imagen trasladada
    return (imgTranslated);
}

/*
* Traslaci�n
*
* Traslaci�n es el desplazamiento de la localizaci�n de un objeto.
*
* Conociendo la cantidad a desplazar (tx,ty) en la direcci�n (x,y), se puede crear la matriz de transformaci�n M como sigue:
*
* M= [ 1  0  tx
*      0  1  ty ]
*
* La matriz M puede construirse con un arreglo Numpy de typo np.float32 y pasarlo como argumento de la funci�n cv.warpAffine().
*
* Se usa la funci�n de transformaci�n Affine de distorsi�n (torcimiento) de la imagen, con nombre y argumentos de entrada:
*
* cv.warpAffine (src, dst, M, dsize, flags = cv.INTER_LINEAR, borderMode = cv.BORDER_CONSTANT, borderValue = new cv.Scalar())
*
* donde los par�metros (argumentos) de entrada son:
*    - src: imagen de entrada (tipo Mat).
*    - dst: imagen de salida de tama�o dsize y mismo tipo que src.
*    - Mat: matriz 2 x 3 de transformaci�n (tipo cv.CV_64FC1).
*    - dsize: tama�o de la imagen de salida.
*    - flags: combinaci�n de m�todos de interpolaci�n (ver cv.InterpolationFlags); donde el m�todo opcional WARP_INVERSE_MAP significa que M es la matriz inversa de transformaci�n ( dst -> src ).
*    - borderMode: m�todo de extrapolaci�n de pixels (ver cv.BorderTypes); cuando borderMode = BORDER_TRANSPARENT, significa que los pixels en la imagen destino correspondiente a los valores at�picos en la imagen origen no son modificados por la funci�n.
*    - borderValue: valor usado en caso de un borde constante; por omisi�n, es cero (color negro).
*/
// Funci�n que traslada una imagen en una cantidad (tx,ty) en pixels eligida por el usuario
Mat trasladar(Mat imgGray, int tx, int ty, int borderMode) {
    // Funci�n que traslada una imagen en una cantidad (tx,ty) en pixels eligida por el usuario

    // Matriz de traslaci�n en cantidades (tx,ty)
    Mat M = (Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);

    // Tama�o de la imagen trasladada igual al tama�o de la imagen original
    Size imgSize = imgGray.size();

    // Llama al constructor normal y crea imagen similar a la original en negro (ceros)
    //Mat imgTransformed(imgSize, imgOriginal.type(), cv::Scalar::all(0));
    Mat imgTransformed;

    // Selecciona si enrolla o si replica el borde de la imagen trasladada
    // borderMode = ((borderMode == cv::BORDER_WRAP) ? cv::BORDER_WRAP : cv::BORDER_REPLICATE);

    // Traslada imagen con transformaci�n Affine de distorsi�n
    warpAffine(imgGray, imgTransformed, M, imgSize, INTER_LINEAR, borderMode);

    imwrite("imagenTrasladada.jpg", imgTransformed);

    // Regresa la imagen trasladada (transformada)
    return (imgTransformed);
}

/*
Mat rotar_ajuste(Mat imgGray, float angulo, int borderMode) {

    Mat imgTransformed;
    
    imgTransformed = trasladar(imgGray, 0, 0 , BORDER_REPLICATE);
    // Punto de rotaci�n (center) en el centro de la imagen original
    
    int current_height = imgGray.rows;
    int current_width = imgGray.cols;
  
    
    //centro de la imagen 
    Point2f centerTransformed((float)((float)imgGray.cols / 2.0), (float)((float)imgGray.rows / 2.0));
    
    imgGray = imgGray(Rect(centerTransformed.x - current_width / 2.0, centerTransformed.y - current_height / 2.0, current_width, current_height));
    
    Mat M = getRotationMatrix2D(centerTransformed, angulo, 1.0); 
    Rect2f bbox = RotatedRect(centerTransformed, imgGray.size(), angulo).boundingRect2f();

    // Selecciona si enrolla o si replica el borde de la imagen trasladada
    borderMode = ((borderMode == cv::BORDER_WRAP) ? cv::BORDER_WRAP : cv::BORDER_REPLICATE);
    
    M.at<double>(0, 2) += bbox.width / 2.0 - imgGray.cols / 2.0;
    M.at<double>(1, 2) += bbox.height / 2.0 - imgGray.rows / 2.0;
    Size imgSize = bbox.size();

    warpAffine(imgGray, imgTransformed, M, imgSize, INTER_LINEAR, borderMode);

    return imgTransformed;

}
*/

Mat rotar_ajuste(Mat imgGray, float angulo, int width, int height, int borderMode) {
    // NO QUITAR LOS SIGUIENTES COMENTARIOS, ES SOLO PARA SABER LAS OPCIONES POSIBLES
    /*
    borderMode = {BORDER_CONSTANT,    //iiiiii|abcdefgh|iiiiiii with some specified i
                  BORDER_REPLICATE,   //aaaaaa|abcdefgh|hhhhhhh
                  BORDER_REFLECT,     //fedcba|abcdefgh|hgfedcb
                  BORDER_WRAP,        //cdefgh|abcdefgh|abcdefg
                  BORDER_REFLECT_101, //gfedcb|abcdefgh|gfedcba
                  BORDER_TRANSPARENT, //uvwxyz|abcdefgh|ijklmno
                  BORDER_REFLECT101,  //same as BORDER_REFLECT_101
                  BORDER_DEFAULT,     //same as BORDER_REFLECT_101
                  BORDER_ISOLATED     //do not look outside of ROI };
    */

    Mat imgRotated;

    // Selecciona si enrolla o si replica el borde de la imagen trasladada
    //borderMode = ((borderMode == cv::BORDER_WRAP) ? cv::BORDER_WRAP : cv::BORDER_REPLICATE);

    //imgGray = trasladar(imgGray, 0, 0, borderMode);
    
    // Punto de rotaci�n (center) en el centro de la imagen original
    int current_height = imgGray.rows;
    int current_width = imgGray.cols;

    //centro de la imagen 
    Point2f centerTransformed((float)((float)imgGray.cols / 2.0), (float)((float)imgGray.rows / 2.0));

    Mat M = getRotationMatrix2D(centerTransformed, angulo, 1.0);
    Rect2f bbox = RotatedRect(centerTransformed, imgGray.size(), angulo).boundingRect2f();

    M.at<double>(0, 2) += bbox.width / 2.0 - imgGray.cols / 2.0;
    M.at<double>(1, 2) += bbox.height / 2.0 - imgGray.rows / 2.0;
    Size imgSize = bbox.size();

    warpAffine(imgGray, imgRotated, M, imgSize, INTER_LINEAR, borderMode);

    // Recorta (ajusta) la imagen al tama�o especificado por (width, height)
    int x; int y;
    x = imgRotated.cols / 2.0 - width / 2.0;
    x = ((x < 0) ? (-x) : (x));
    y = imgRotated.rows / 2.0 - height / 2.0;
    y= ((y < 0) ? (-y) : (y));
    
    Mat imgTransformed(imgRotated, Rect(x, y, width, height));

    imwrite("imagenRotarAjuste.jpg", imgTransformed);
    return imgTransformed;
}

// Funci�n que rota una imagen en un �ngulo en grados eligido por el usuario
Mat rotar(Mat imgGray, float angulo, int borderMode) {
    // Funci�n que rota una imagen en un �ngulo en grados eligido por el usuario

    //float angulo = -45;

    // Punto de rotaci�n (center) en el centro de la imagen original
    int current_height = imgGray.rows;
    int current_width = imgGray.cols;

    Point2f center((float)((float)imgGray.cols / 2.0), (float)((float)imgGray.rows / 2.0));

    // Matriz de rotaci�n en (angulo) grados, posicionada en (center) con escalamiento (1.0)
    Mat M = getRotationMatrix2D(center, angulo, 1.0);
    // Rect�ngulo m�nimo que contiene al rect�ngulo rotado
    Rect2f bbox = RotatedRect(center, imgGray.size(), angulo).boundingRect2f();
    // Actualiza la escala de la matriz de rotaci�n
    M.at<double>(0, 2) += bbox.width / 2.0 - imgGray.cols / 2.0;
    M.at<double>(1, 2) += bbox.height / 2.0 - imgGray.rows / 2.0;

    // Tama�o de la imagen rotada
    Size imgSize = bbox.size();

    // Selecciona si enrolla o si replica el borde de la imagen trasladada
    //borderMode = ((borderMode == cv::BORDER_WRAP) ? cv::BORDER_WRAP : cv::BORDER_REPLICATE);

    // Llama al constructor normal y crea imagen similar a la original en negro (ceros)
    //Mat imgTransformed(imgOriginal.size(), imgOriginal.type(), cv::Scalar::all(0))
    
    Mat imgTransformed;
   
    // Rota imagen con transformaci�n Affine de distorsi�n
    //warpAffine(imgGray, imgTransformed, M, imgSize, INTER_LINEAR, BORDER_CONSTANT);
    //warpAffine(imgGray, imgTransformed, M, imgSize, INTER_LINEAR, BORDER_WRAP);
    warpAffine(imgGray, imgTransformed, M, imgSize, INTER_LINEAR, borderMode);


    //Point2f centerTransformed((float)((float)imgTransformed.cols/ 2.0), (float)((float)imgTransformed.rows / 2.0));
    //Rect2f RectTrasnformed= RotatedRect(centerTransformed, imgTransformed.size(),angulo).boundingRect2f();
    // Recorta la imagen rotada al tama�o de la imagen original
    //Point2f centerTransformed((float)((float)imgTransformed.cols / 2.0), (float)((float)imgTransformed.rows / 2.0));
    //imgTransformed = imgTransformed(Rect(centerTransformed.x - current_width / 2.0, centerTransformed.y - current_height / 2.0, current_width, current_height));
    //rombo-
    // Regresa la imagen rotada (transformada)

    
    imwrite("imagenRotar.jpg", imgTransformed);
    return (imgTransformed);
}

Mat escalar_ajuste(Mat imgGray, int new_height, int new_width, int borderMode) {

    Mat imgScaled;
    Mat imgTransformed;
  
    // Tama�o de la imagen trasladada igual al tama�o de la imagen original
    
    Size imgSize = imgGray.size();
    // Obtiene las dimensiones de la imagen original
    int height = imgGray.rows;
    int width = imgGray.cols;

    // Si los tama�os son iguales, no hace nada, regresa una copia
    if (((int) new_height == height) && ((int) new_width == width))
    {
        imgGray.copyTo(imgScaled);
    }

    // El usuario al ingresar los datos, los condicionamos si es mayor aplicamos 
    // una interpolaci�n INTER_AREA de lo contrario, por un else, se aplica la 
    // interpolaci�n INTER_LINEAR
    if ((int)new_height > height || (int)new_width > width)
    {
        cv::resize(imgGray, imgScaled, Size((int)new_height, (int)new_width), 0, 0, INTER_LINEAR);
    }
    else
    {
        cv::resize(imgGray, imgScaled, Size((int)new_height, (int)new_width), 0, 0, INTER_AREA);
    }


    // Recorta (ajusta) la imagen al tama�o especificado por (width, height)
    
    int x; int y;
    Mat empty(width, height, CV_8UC3, Scalar(0, 0, 0));
    cout << "imgGray ancho = " << width << ", imgGray alto = " << height << endl << endl;
    cout << "imgScaled ancho = " << imgScaled.rows << ", imgScaled alto = " << imgScaled.cols << endl << endl;
    cout << "nuevo ancho = " << new_width << ", nuevo alto = " << new_height << endl << endl;
    cout << "empty ancho = " << empty.rows << ", empty alto = " << empty.cols << endl << endl;
   
    cout << "Oprima una tecla y [ENTER] para continuar procesando escalar_ajuste()... ";
    int keyPAUSE; cin >> keyPAUSE;
    cout << endl;

    /*
    scr.copy.to(dst(Rect(left, top, src.cols, src.crows)))

    imgScaled.copyTo(empty(Rect(x, y, new_width, new_height));
    trasladar(empty, 1, 1, BORDER_REPLICATE);
    */

    if (width > new_width && height > new_height) {
        x = imgScaled.cols / 2.0 - width / 2.0;
        y = imgScaled.rows / 2.0 - height / 2.0;
        x = ((x < 0) ? (-x) : (x));
        y = ((y < 0) ? (-y) : (y));
        //cout << "x = " << x << ", y = " << y << endl << endl;

        //scr.copy.to(dst(Rect(left, top, src.cols, src.crows)))
        imgScaled.copyTo(empty(Rect(x, y, new_width, new_height)));
        Mat imgTransformed = trasladar(empty, 1, 1, borderMode);

        cout << "imgTransformed ancho = " << imgTransformed.rows << ", imgTransformed alto = " << imgTransformed.cols << endl << endl;
        cout << "Oprima una tecla y [ENTER] para continuar procesando escalar_ajuste()... ";
        int keyPAUSE; cin >> keyPAUSE;
        cout << endl;

        //Mat imgTransformed
        //imgTransformed(imgScaled, Rect(x, y, width, height));
        
        //warpAffine(imgScaled, imgTransformed,,);

        /*
        x = imgScaled.cols *  2.0  + width * 2.0;
        x = ((x < 0) ? (-x) : (x));
        y = imgScaled.rows * 2.0 + height * 2.0;
        y = ((y < 0) ? (-y) : (y));*/

        //Mat imgTransformed(imgScaled, Rect(x, y, width, height));
       
    }
    else if ( width > new_width && height < new_height) {
        x = imgScaled.cols * 2.0 + width * 2.0;
        x = ((x < 0) ? (-x) : (x));
        y = imgScaled.rows / 2.0 - height / 2.0;
        y = ((y < 0) ? (-y) : (y));

        Mat imgTransformed(imgScaled, Rect(x, y, width, height));

    }
    else if (width < new_width && height > new_height) {
        x = imgScaled.cols / 2.0 - width / 2.0;
        x = ((x < 0) ? (-x) : (x));
        y = imgScaled.rows * 2.0 + height * 2.0;
        y = ((y < 0) ? (-y) : (y));

        Mat imgTransformed(imgScaled, Rect(x, y, width, height));

    }
    else if ( width < new_width && height < new_height) {
        x = imgScaled.cols / 2.0 - width / 2.0;
        x = ((x < 0) ? (-x) : (x));
        y = imgScaled.rows / 2.0 - height / 2.0;
        y = ((y < 0) ? (-y) : (y));

        Mat imgTransformed(imgScaled, Rect(x, y, width, height));

    }
    
    return (imgTransformed);
}

Mat escalar(Mat imgGray, int new_heigth, int new_width, int borderMode) {

    Mat imgTransformed;

    // Tama�o de la imagen trasladada igual al tama�o de la imagen original
    Size imgSize = imgGray.size();
    // Obtiene las dimensiones de la imagen original
    int current_height = imgGray.rows;
    int current_width = imgGray.cols;

    // Si los tama�os son iguales, no hace nada, regresa una copia
    if (((int)new_heigth == current_height) && ((int)new_width == current_width))
    {
        imgGray.copyTo(imgTransformed);
    }

    // El usuario al ingresar los datos, los condicionamos si es mayor aplicamos 
    // una interpolaci�n INTER_AREA de lo contrario, por un else, se aplica la 
    // interpolaci�n INTER_LINEAR
    if ((int)new_heigth > current_height || (int)new_width > current_width)
    {
        cv::resize(imgGray, imgTransformed, Size((int)new_heigth, (int)new_width), 0, 0, INTER_LINEAR);
    }
    else
    {
        cv::resize(imgGray, imgTransformed, Size((int)new_heigth, (int)new_width), 0, 0, INTER_AREA);
    }

    imwrite("imagenEscalada.jpg",imgTransformed );

    return (imgTransformed);
}

Mat escala_grises(Mat imgOriginal) {

    Mat imgGrayscale;

    cvtColor(imgOriginal, imgGrayscale, COLOR_BGR2GRAY);
    return (imgGrayscale);
}

Mat binarizar(Mat imgGray) {

    Mat imgGrayscale;
   
    Mat imgBiranizar;

    double thresh = 127;// Para el negro, si este valor esta en 0 la imagen sera completamente blanca
    double maxValue = 255;// para el blanco

    cvtColor(imgGray, imgGrayscale, COLOR_BGR2GRAY);
    threshold(imgGrayscale, imgBiranizar, thresh, maxValue, THRESH_BINARY);
    imwrite("imagenbinarizada.jpg", imgBiranizar);
    return (imgBiranizar);
}

void reordenar_indices_imagen(std::array<int, 16>& orden_aleatorio) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(orden_aleatorio.begin(), orden_aleatorio.end(), std::default_random_engine(seed));
    
    std::cout << "shuffled elements:";
    
    for (int& x: orden_aleatorio) std::cout << ' ' << x;
    std::cout << '\n';

}
    /*
    
    
    //int n = sizeof(orden_original) / sizeof(orden_original[0]);
    int min = 1, max = 16;
    int i = 1;
    unsigned seed = 0;
    //shuffle(orden_original, orden_original + n, default_random_engine(seed));
    
    //shuffle(indice_original.begin(), indice_original.end(), std::default_random_engine(indiceAleatorio));
    std::cout << "Elementos a revolver:";
    //for (int& x : indice_original) std::cout << ' ' << x;
    std::cout << '\n';

  
    //srand(time(NULL));
    //random_device indiceAletorio; //Variable aletoria
    //mt19937 rango(indiceAletorio());// Algoritmo Mersenne twister: http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/ARTICLES/mt.pdf 
    //uniform_int_distribution<> distribucion(min, max);//Distribuci�n Unifomre del 1 al 16
    //int ValorAleatorio = distribucion(rango);
    
    //int  range = (max - min);
    //indiceAleatorio =  rand() % 16;
   
    //cout << "El indice aleatorio es: " << ValorAletorio << endl;
    //orden_aleatorio[0] = ValorAleatorio;
    /*
    for (int i = 0; i < n; i++) {
        cout << orden_orginal[i] << " ";

    }
    cout << endl;

    
    
    /*while(i < 16){
        /*if (orden_aleatorio[i] == '\0') {
            orden_aleatorio[i] = ValorAleatorio;
            cout << "arreglo orden aleatorio:  " << orden_aleatorio[i] << endl;
            i++;
        

        if(  ValorAleatorio != orden_aleatorio[i]){
            orden_aleatorio[i]= ValorAleatorio;
            cout << "arreglo orden aleatorio:  " << orden_aleatorio[i] << endl;
            i++;
       
        }
        cout << "arreglo orden_original: " << orden_orginal[i] << endl;
    }*/
    //cout << "Hola:" <<endl;
    //cin >> x;
//}
/*
}
*/

Mat recortar_imagen(Mat imgOriginal, int row, int col) {
    // (row, col) son par�metros, cada uno con valores del 1 al 4
    // Representan el indice (renglon, columna) de la subimagen recortada

    // Dimensiones de la imagen original
    int current_height = imgOriginal.rows;
    int current_width = imgOriginal.cols;
    // Dimensiones de la imagen recortada (subimagen)
    int height = current_height / 4;
    int width = current_width / 4;
    // Posicion inicial esquina superior izquierda (x,y)
    // del recuadro de recorte con ancho y alto (width,height)
    int x = width * (col - 1);
    int y = height * (row - 1);

    // Imprime coordenadas del recuadro del recorte
    cout << "Posicion inicial esquina superior izquierda (x,y) ";
    cout << "del recuadro de recorte con ancho y alto (width, height)" << endl << endl;
    cout << "(row, col) = (" << row << ", " << col << ")" << endl;
    cout << "(x, y, width, height) = (";
    cout << x << ", " << y << ", " << width << ", " << height << ")" << endl << endl;

    // Recorta image en el recuadro especificado (subimagen)
    Mat imgGray(imgOriginal, Rect(x, y, width, height));

    //Regresa imagen recortada (subimagen)
    imwrite("imgGray.png", imgGray);
    return (imgGray);      
}

Mat unir_imagen(Mat imgGray, int k) {
    
    static Mat image_unir;
    int  f, num_img;// col, row;
    //std::array<int, 4> transformaciones = { 1,2,3,4 };
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //shuffle(transformaciones.begin(), transformaciones.end(), std::default_random_engine(seed));
    
    for (f = 0; f < 16; f++) {
        num_img = orden_aleatorio[f];

       
        
    }
    
    return image_unir;
}

// Muestra los indices (row,col) de la subimagen a partir de su indice num_img
void show_indexes(void)
{
    // Muestra los indices (row,col) de la subimagen a partir de su indice num_img
    cout << endl << "Indice num_img de la subimagen y sus indices (row,col) calculados" << endl << endl;
    for (int k = 0; k < 16; k++) {
        int num_img, row, col;
        num_img = k + 1;
        // Obtiene el n�mero de rengl�n y de columna dado el n�mero de imagen
        col = ((num_img % 4) ? (num_img % 4) : 4);
        row = ((num_img - col) / 4) + 1;
        if (num_img == 1) {
            cout << "num_img (row,col) = " << endl;
            cout << num_img << " (" << row << "," << col << "), ";
        }
        if ((num_img > 1) && (num_img < 16)) {
            cout << num_img << " (" << row << "," << col << "), ";
            if (col == 4) {
                cout << endl;
            }
        }
        if (num_img == 16) {
            cout << num_img << " (" << row << "," << col << ")" << endl;
        }
    }
    cout << endl;
}


/*Function///////////////////////////////////////////////////////////////
[url]https://github.com/opencv/opencv/wiki/DisplayManyImages [\url]

Name:       ShowManyImages

Purpose:

This is a function to display more than one
image in a single window using Intel OpenCV

Parameters:

string title: Title of the window to be displayed
int    nArgs: Number of images to be displayed
Mat    img1: First Mat, which contains the first image
...
Mat    imgN: First Mat, which contains the Nth image

Language:   C++

The method used is to set the ROIs of a Single Big image and then resizing
and copying the input images on to the Single Big Image.

This function does not stretch the image...
It resizes the image without modifying the width/height ratio..

This function can be called like this:

Mat img1 = imread("image1.png");
Mat img2 = imread("image2.png");
Mat img3 = imread("image3.png");
ShowManyImages("Images", 3, img1, img2, img3);

This function can display upto 12 images in a single window.
It does not check whether the arguments are of type Mat or not.
The maximum window size is 700 by 660 pixels.
Does not display anything if the number of arguments is less than
one or greater than 12.

Idea was from [[BettySanchi]] of OpenCV Yahoo! Groups.

If you have trouble compiling and/or executing
this code, I would like to hear about it.

You could try posting on the OpenCV Yahoo! Groups
[url]http://groups.yahoo.com/group/OpenCV/messages/ [/url]


Parameswaran,
Chennai, India.

cegparamesh[at]gmail[dot]com

...
///////////////////////////////////////////////////////////////////////*/
/*
void ShowManyImages(string title, int nArgs, ...) {
    int size;
    int i;
    int m, n;
    int x, y;

    // w - Maximum number of images in a row
    // h - Maximum number of images in a column
    int w, h;

    // scale - How much we have to resize the image
    float scale;
    int max;

    // If the number of arguments is lesser than 0 or greater than 12
    // return without displaying
    if (nArgs <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if (nArgs > 14) {
        printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
        return;
    }
    // Determine the size of the image,
    // and the number of rows/cols
    // from number of arguments
    else if (nArgs == 1) {
        w = h = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        w = 2; h = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        w = 2; h = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        w = 3; h = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        w = 4; h = 2;
        size = 200;
    }
    else {
        w = 4; h = 3;
        size = 150;
    }

    // Create a new 3 channel image
    Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC3);

    // Used to get the arguments passed
    va_list args;
    va_start(args, nArgs);

    // Loop for nArgs number of arguments
    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
        // Get the Pointer to the IplImage
        Mat img = va_arg(args, Mat);

        // Check whether it is NULL or not
        // If it is NULL, release the image, and return
        if (img.empty()) {
            printf("Invalid arguments");
            return;
        }

        // Find the width and height of the image
        x = img.cols;
        y = img.rows;

        // Find whether height or width is greater in order to resize the image
        max = (x > y) ? x : y;

        // Find the scaling factor to resize the image
        scale = (float)((float)max / size);

        // Used to Align the images
        if (i % w == 0 && m != 20) {
            m = 20;
            n += 20 + size;
        }

        // Set the image ROI to display the current image
        // Resize the input image and copy the it to the Single Big Image
        Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
        Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
        temp.copyTo(DispImage(ROI));
    }

    // Create a new window, and show the Single Big Image
    namedWindow(title, 1);
    imshow(title, DispImage);
    waitKey();

    // End the number of arguments
    va_end(args);
}
*/