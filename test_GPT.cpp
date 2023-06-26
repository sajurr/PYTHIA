#include <iostream>
#include <vector>
#include "TH1F.h"
#include "TCanvas.h"

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Creating a histogram
    TH1F* histogram = new TH1F("histogram", "Selected Numbers", 10, 0.5, 10.5);

    // Filling the histogram with the selected elements
    for (size_t i = 0; i < numbers.size(); ++i) 
    {
        int number = numbers[i];
        if (number % 2 == 0) 
        {
            histogram->Fill(i + 1, number);
        }
    }

    // Creating a canvas and drawing the histogram
    TCanvas* canvas = new TCanvas("canvas", "Histogram", 800, 600);
    histogram->Draw("bar");

    // Running the event loop
    canvas->Update();
    canvas->Modified();
    canvas->SaveAs("histogram.png");

    return 0;
}
