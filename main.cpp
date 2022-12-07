#include "libs.h"
using namespace std;
int main() {
    srand(time(NULL));
    vector<string> input = read_image_input();
    int testNumber;
    cout << "Test Number \n";
    cin >> testNumber;
    vector<string> noisy_input = read_noisy_image_input(testNumber);

    vector<vector<int>> noisy_images = convert_imgs_to_vector(noisy_input);
    vector<vector<int>> learning_sample = convert_imgs_to_vector(input);

    for (int test = 0; test < noisy_input.size(); test++) {
        cout << "\n--------Test " << test << "--------" << endl;
        cout << "Test image:" << endl << endl;
        cout << noisy_input.at(test) << endl << endl;

        vector<unsigned int> winners = process(noisy_images.at(test), learning_sample);
        int winner;
        if (winners.size() > 1) {
            winner = winners[rand() % winners.size()];
            cout << "Possible numbers: ";
            for (int num = 0; num < winners.size(); num++) {
                cout << winners.at(num) << " ";
            }
            cout << endl;
        }
        else winner = winners[0];
        cout << "It is number '" << winner << "':\n" << input[winner] << endl;
    }




}
