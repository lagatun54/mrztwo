//
// Created by Prokhar Kulak on 7.12.22.
//

#ifndef MRZ_3_LIBS_H
#define MRZ_3_LIBS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#include <time.h>
#include <cmath>

const int HEIGHT = 5;
const int WIDTH = 5;
int n = HEIGHT * WIDTH;
int m = 10;

using namespace std;


vector<unsigned int> find_winners(vector<double> output) {
    vector<unsigned int> result;
    for (int i = 0; i < output.size(); i++) {
        if (output[i] > 0)
            result.push_back(i);
    }
    return result;
}

double activation_function(double argument) {
    return argument >= 0 ? argument : 0;
}
string read_input_file(string filename) {   //считывание входного файла

    ifstream input_file(filename + ".txt");
    string input((istreambuf_iterator<char>(input_file)),
                 istreambuf_iterator<char>());
    input_file.close();
    return input;
}

vector<string> read_image_input() {
    vector <string> input;
    for (int filename = 0; filename <= 9; filename++) {
        string image = read_input_file("learning_images/" + to_string(filename));
        input.push_back(image);
    }
    return input;
}

vector<string> read_noisy_image_input(int filename) {
    vector <string> input;
    string image = read_input_file("noisy_images/" + to_string(filename));
    input.push_back(image);
    return input;
}

vector<int> parse_learning_image(string image) {    //представление изобр в вектор
    vector<int> result;
    for (int pixel = 0; pixel < image.size(); ++pixel) {
        char pixel_character = image[pixel];
        switch (pixel_character) {
            case '.':
                result.push_back(-1);
                break;
            case '#':
                result.push_back(1);
                break;
        }
    }
    return result;
}

vector<vector<int>> convert_imgs_to_vector(vector<string> image_input) { //создание вектора 10 векторов
    vector<vector<int>> vector_images;
    for (vector<string>::iterator image = image_input.begin(); image != image_input.end(); image++) {
        vector_images.push_back(parse_learning_image(*image));
    }
    return vector_images;
}



int hamming_distance(vector<int> first, vector<int> second) { //вычисление рассто€ни€ ’эмминга

    int distance = 0;
    for (int bit = 0; bit < first.size(); ++bit) {
        if (first[bit] != second[bit])
            distance++;
    }
    return distance;
}

vector<unsigned int> process(vector<int> noisy_image, vector<vector<int>> learning_sample) { //само обучение
    vector<int> Y(m);
    for (int i = 0; i < m; i++) {
        Y[i] = n - hamming_distance(noisy_image, learning_sample.at(i));
    }
    vector<double> Z(m);
    for (int j = 0; j < m; j++) {
        Z[j] = Y[j] >= 0 ? Y[j] : 0;
    }
    double eps = 0.095;
    vector<unsigned int> winners(m, 0);
    vector<double> Znext = Z;
    int it = 0;

    double Emax = 0.01;
    do {
        it++;
        Z = Znext;
        vector<double> E(m, 0);
        for (int j = 0; j < m; j++) {
            double sum = 0;
            for (int k = 0; k < m; k++) {
                if (k != j) {
                    sum += Z[k];
                }
            }
            double value = Z[j] - eps * sum;
            Znext[j] = activation_function(value);

            E.at(j) = abs(Znext[j] - Z[j]);
        }

        double diff = 0;
        for (int j = 0; j < m; ++j) {
            diff += E.at(j)*E.at(j);
        }
        diff = sqrt(diff);
        winners = find_winners(Znext);

        if (diff <= Emax) {
            break;
        }
    } while (winners.size() > 1);
    cout << "Iterations: " << it << endl;
    return winners;
}
#endif //MRZ_3_LIBS_H
