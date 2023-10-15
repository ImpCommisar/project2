// project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Javier David H. Bolong
// Johnvee Mandal
// justine Sison

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
using namespace std;




class sig
{
public:
    double start_freq = 0;
    double end_freq = 0;
    double sampling_rate = 0;
    double duration = 0;
    int steps = 0;
    double stepsize = 0;
    vector<double> placeholder;

    bool index_check(string input, double& number)
    {
        stringstream ss(input);

        ss >> number;

        if (!ss.fail() && ss.eof())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool importSignal(string file_name)
    {
        ifstream input;
        string line;
        string index_test;
        string test;
        stringstream ss;
        stringstream ss2;
        bool index_checked;
        bool first_signal_checked = false;
        double test_double;
        input.open(file_name);
        if (!input)
        {
            cout << "Unable to open " << file_name << " check file format or contents...\n";
            return false;
        }

        getline(input, line);

        if (line.empty())
        {
            cout << "1st line empty, cutting importing process...\n";
            return false;
        }

        ss << line;

        // first pass
        ss >> test;
        ss2 << test;
        cout << test << endl;
        if (ss2 >> test_double)
        {
            if (ss2.eof())
            {
                if (test_double == floor(test_double))
                {
                    cout << "Valid Index found, setting index to 0, checking for signal...\n";
                }
                else
                {
                    cout << "Valid signal value detected instead of valid index, setting index to 0, importing signal value...\n";
                    first_signal_checked = true;
                    placeholder.push_back(test_double);
                }
            }
            else
            {
                cout << "invalid number detected, with extra characters from: " << test << ", thus invalid input cutting importing... \n";
                return false;
            }
        }
        else
        {
            cout << test << " is not a number, cutting signal process...\n";
            return false;
        }
        ss2.clear();


        //2nd pass

        ss >> test;

        ss2 << test;
        cout << test << endl;
        if (ss2 >> test_double && first_signal_checked == false)
        {
            if (ss2.eof())
            {
                cout << "Obtained floating pt number " << test_double << endl;
                placeholder.push_back(test_double);
            }
            else
            {
                cout << "invalid number detected, with extra characters from: " << test << ", thus invalid input cutting importing... \n";
                return false;
            }
        }
        else if (first_signal_checked == false)
        {
            cout << test << " is not a number, cutting signal process...\n";
            return false;
        }
        ss.clear();
        ss2.clear();

        //checking the rest of the values in the input
        while (!input.eof())
        {
            getline(input, line);
            if (line.length() == 0)
            {
                cout << "cut on signal detected, stopping process... \n";
                return true;
            }

            ss << line;

            if (ss >> test_double)
            {
                if (ss.eof())
                {
                    cout << "Obtained floating pt number " << test_double << endl;
                    placeholder.push_back(test_double);
                }
                else
                {
                    cout << "invalid floating pt number: " << test_double << endl;
                    cout << "invalid signal detected, cutting process..." << endl;
                    return false;
                }
            }
            ss.str("");
            ss.clear();
        }

        input.close();
        return true;
    }
    bool exportSignal(string file_name, double real[], double imaginary[], double magnitude[], double phase[], int size_sig, double startFreq, double stepsize)
    {
        ofstream output;
        output.open(file_name);

        if (!output)
        {
            cout << file_name << " could not be opened, check file_name or format...\n";
        }
        else
        {
            cout << "frequency (Hz)\t real part\t imaginary\n";
            for (int j = 0; j < size_sig; j++)
            {

            }

            for (int j = 0; j < size_sig; j++)
            {

            }
        }
        output.close();
    }
    double obtain_stepsize(double startFreq, double endFreq, int steps)
    {
        double stepsize = 0;

        stepsize = (endFreq - startFreq) / steps;

        return stepsize;
    }
};

double digitize(double analogFreq, double samplingFreq)
{
    double Pi = 3.14159265358979;
    return 2 * Pi * (analogFreq / samplingFreq);
}
// Function for the real part
double cosineCoeff(double signalData[], double digitalFreq, int duration)
{
    double cosCoeff = 0;
    for (int i = 0; i < duration; i++)
    {
        cosCoeff += signalData[i] * cos(digitalFreq * i);
    }

    // Normalizing the real part
    //return cosCoeff / sqrt(cosine_autocorr * input_autocorr);
    return cosCoeff;
}

// Function for the imaginary part
double sineCoeff(double  signalData[], double digitalFreq, int duration)
{
    double sinCoeff = 0;
    for (int i = 0; i < duration; i++)
    {
        sinCoeff += signalData[i] * sin(digitalFreq * i);
    }

    // Normalizing the imaginary part
    //return sinCoeff / sqrt(sine_autocorr * input_autocorr);
    return sinCoeff;
}

double estimate_low(double number)
{
    if (number <= 0.000001 && number >= 0)
    {
        return 0;
    }
    else if (number >= -0.000001 && number < 0)
    {
        return 0;
    }

    else
    {
        return number;
    }
}

void DFT(
    double data[], int duration,
    double sampling_rate, double startFreq, double endFreq,
    int steps, double stepsize,
    double real[], double imaginary[], double magnitude[], double phase[])
{
    double analogFreq = startFreq;
    double Pi = 3.14159265358979;

    double rl = 0, img = 0, mag = 0, phas = 0;

    double digitalFreq = digitize(analogFreq, sampling_rate);
    for (int i = 0; i <= duration; i++)
    {
        cout << data[i] << endl;
    }

    for (int i = 0; i <= steps; i++)
    {
        double digitalFreq = digitize(analogFreq, sampling_rate);

        rl = cosineCoeff(data, digitalFreq, duration);
        img = sineCoeff(data, digitalFreq, duration) * -1;
        mag = sqrt((img * img) + (rl * rl));
        phas = atan2(img, rl) * (180 / Pi);


        real[i] = estimate_low(rl);
        imaginary[i] = estimate_low(img);
        magnitude[i] = estimate_low(mag);
        phase[i] = estimate_low(phas);

        analogFreq += stepsize;
    }
}


int main(int argc, char* argv[])
{
    sig project;
    string exit;

    int size_signal = 0;
    if (argc == 6)
    {
        project.importSignal(argv[1]);
        project.sampling_rate = stod(argv[2]);
        project.start_freq = stod(argv[3]);
        project.end_freq = stod(argv[4]);
        project.steps = stod(argv[5]);
        project.duration = project.placeholder.size();
        project.stepsize = project.obtain_stepsize(project.start_freq, project.end_freq, project.steps);
    }
    else
    {
        cout << "Missing command arguements, there should be 6 arguements alongside the .exe declaration...\n";
        return 1;
    }

    double* data = new double[project.steps];
    double* real = new double[project.steps];
    double* imaginary = new double[project.steps];
    double* magnitude = new double[project.steps];
    double* phase = new double[project.steps];
    double hold_round_num;
    size_signal = project.duration;

    cout << project.duration << endl;

    //problem
    /*
    for (int j = 0; j <= size_signal; j++)
    {
        data[j] = project.placeholder[j];
    }
    */
    DFT(data, project.duration, project.sampling_rate, project.start_freq, project.end_freq, project.steps, project.stepsize, real, imaginary, magnitude, phase);

    //quarantined problem
    /*
    for (int j = 0; j <= size_signal; j++)
    {
        data[j] = project.placeholder[j];
    }

    DFT(data, project.duration, project.sampling_rate, project.start_freq, project.end_freq, project.steps, project.stepsize, real, imaginary, magnitude, phase);

    for (int j = 0; j <= project.steps; j++)
    {
        cout << "real[" << j << "]: " << real[j] << endl;
    }
    cout << "\n";

    for (int j = 0; j <= project.steps; j++)
    {
        cout << "imaginary[" << j << "]: " << imaginary[j] << endl;
    }

    cout << "\n";

    for (int j = 0; j <= project.steps; j++)
    {
        cout << "magnitude[" << j << "]: " << magnitude[j] << endl;
    }

    cout << "\n";

    for (int j = 0; j <= project.steps; j++)
    {
        cout << "phase[" << j << "]: " << phase[j] << endl;
    }

    cout << "\n";
    //project.exportSignal(argv[6], real, imaginary, magnitude, phase, project.steps);
    */
}


