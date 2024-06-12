#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Structure for Dissolved Gas Analysis (DGA)
struct DGA {
    float H2, CH4, C2H6, C2H4, C2H2, CO, CO2;
};

// Function to evaluate DGA conditions
string evaluateDGA(const DGA &dga) {
    if (dga.H2 < 100 && dga.CH4 < 50 && dga.C2H6 < 65 && dga.C2H4 < 150 && dga.C2H2 < 1 && dga.CO < 500) {
        return "Normal: No significant issues.";
    } else if (dga.H2 >= 100 && dga.H2 <= 700 && dga.CH4 < 120 && dga.C2H6 < 65 && dga.C2H4 < 150 && dga.C2H2 < 1 && dga.CO < 500) {
        return "Partial Discharges: Corona, low energy discharges.";
    } else if (dga.H2 >= 100 && dga.H2 <= 700 && dga.CH4 >= 120 && dga.CH4 <= 400 && dga.C2H6 < 65 && dga.C2H4 < 150 && dga.C2H2 < 1 && dga.CO < 500) {
        return "Sparking < 150°C: Low-energy arcing.";
    } else if (dga.H2 < 50 && dga.CH4 >= 120 && dga.CH4 <= 400 && dga.C2H6 >= 65 && dga.C2H6 <= 100 && dga.C2H4 >= 150 && dga.C2H4 <= 200 && dga.C2H2 < 1 && dga.CO < 500) {
        return "Local Overheating 150-300°C: Thermal faults below 300°C.";
    } else if (dga.H2 < 50 && dga.CH4 < 120 && dga.C2H6 >= 100 && dga.C2H6 <= 150 && dga.C2H4 >= 200 && dga.C2H4 <= 350 && dga.C2H2 < 1 && dga.CO < 500) {
        return "Severe Overheating 300-700°C: Thermal faults 300-700°C.";
    } else if (dga.H2 >= 100 && dga.H2 <= 700 && dga.CH4 >= 400 && dga.CH4 <= 1000 && dga.C2H6 < 65 && dga.C2H4 >= 200 && dga.C2H4 <= 350 && dga.C2H2 >= 35 && dga.C2H2 <= 700 && dga.CO < 500) {
        return "Arcing > 700°C: High-energy arcing.";
    } else if (dga.H2 < 100 && dga.CH4 < 50 && dga.C2H6 < 65 && dga.C2H4 < 150 && dga.C2H2 < 1 && dga.CO > 500) {
        return "Cellulose Degradation: Insulation degradation.";
    }

    float ratios[] = {dga.CH4 / 40, dga.C2H6 / 50, dga.C2H4 / 60, dga.C2H2 / 1, dga.H2 / 60};
    float max_ratio = *max_element(ratios, ratios + 5);

    if (max_ratio == dga.H2 / 60) {
        return "Case-1: Partial Discharges in voids";
    } else if (max_ratio == dga.CH4 / 40) {
        return "Case-2: Sparking < 150°C";
    } else if (max_ratio == dga.C2H6 / 50) {
        return "Case-3: Local Overheating between 150°C and 300°C";
    } else if (max_ratio == dga.C2H4 / 60) {
        return "Case-4: Severe Overheating between 300°C and 700°C";
    } else if (max_ratio == dga.C2H2 / 1) {
        return "Case-5: Arcing > 700°C";
    }

    return "Unknown condition.";
}

// Function to evaluate IEC conditions
string evaluateIEC(float C10, float C11, float C12) {
    int D10 = (C10 < 0.1) ? 0 : (C10 < 1) ? 1 : (C10 < 3) ? 1 : 2;
    int D11 = (C11 < 0.1) ? 1 : (C11 < 1) ? 0 : (C11 < 3) ? 2 : 2;
    int D12 = (C12 < 0.1) ? 0 : (C12 < 1) ? 0 : (C12 < 3) ? 1 : 2;

    if (D10 == 0 && D11 == 0 && D12 == 0) {
        return "Case - 0: No Fault";
    } else if (D10 == 0 && D11 == 1 && D12 == 0) {
        return "Case 1: Low energy Partial Discharges";
    } else if (D10 == 1 && D11 == 1 && D12 == 0) {
        return "Case 2: Low Energy Partial Discharges with tracking";
    } else if ((D10 == 1 || D10 == 2) && D11 == 0 && (D12 == 1 || D12 == 2)) {
        return "Case - 3 : Discharges of Low Energy Density (Sparking)";
    } else if (D10 == 1 && D11 == 0 && D12 == 2) {
        return "Case 4: Discharges of High Energy (Arcing)";
    } else if (D10 == 0 && D11 == 0 && D12 == 1) {
        return "Case 5: Thermal Fault < 150°C - General insulated conductor overheating";
    } else if (D10 == 0 && D11 == 2 && D12 == 0) {
        return "Case 6: Thermal Fault 150°C - 300°C";
    } else if (D10 == 0 && D11 == 2 && D12 == 1) {
        return "Case 7: Thermal Fault 300°C - 700°C";
    } else {
        return "Case 8: Thermal Fault > 700°C";
    }
}

// Function to evaluate Roger conditions
string evaluateRoger(float C10a, float C11a, float C12a, float C13a) {
    if (C10a > 0.1 && C10a < 1 && C12a < 1 && C13a < 0.1) {
        return "Case-0: Normal";
    } else if (C10a < 0.1 && C12a < 1 && C13a < 0.1) {
        return "Case-1: Low Energy density PD";
    } else if (C10a > 0.1 && C10a < 1 && C12a > 3 && C13a >= 0.1 && C13a <= 3) {
        return "Case-2: Arcing High Energy discharge";
    } else if (C10a > 0.1 && C10a < 1 && C12a >= 1 && C12a <= 3 && C13a < 0.1) {
        return "Case-3: Low temperature thermal";
    } else if (C10a > 1 && C12a >= 1 && C12a <= 3 && C13a < 0.1) {
        return "Case-4: Thermal Fault < 700 C";
    } else {
        return "Case-5: Thermal Fault > 700 C";
    }
}

// Function to evaluate Roger4 conditions
string evaluateRoger4(float C10a, float C11a, float C12a, float C13a) {
    int D10 = (C10a < 0.1) ? 5 : (C10a < 1) ? 0 : (C10a < 3) ? 2 : 2;
    int D11 = (C10a < 1) ? 0 : (C11a < 1) ? 1 : 1;
    int D12 = (C12a < 1) ? 0 : (C12a < 3) ? 1 : (C12a < 3) ? 2 : 2;
    int D13 = (C13a < 0.5) ? 0 : (C13a < 3) ? 1 : (C13a < 3) ? 2 : 2;

    if (D10 == 0 && D11 == 0 && D12 == 0 && D13 == 0) {
        return "Case-0: Normal";
    } else if (D10 == 5 && D11 == 0 && D12 == 0 && D13 == 0) {
        return "Case-1: Partial Discharge";
    } else if ((D10 == 1 || D10 == 2) && D11 == 0 && D12 == 0 && D13 == 0) {
        return "Case-2: Slight Overheating < 150 C";
    } else if ((D10 == 1 || D10 == 2) && D11 == 1 && D12 == 0 && D13 == 0) {
         return "Case-3: Overheating 150 C - 200 C";
    } else if (D10 == 0 && D11 == 1 && D12 == 0 && D13 == 0) {
        return "Case-4: Overheating 200 C - 300 C";
    } else if (D10 == 0 && D11 == 0 && D12 == 1 && D13 == 0) {
        return "Case-5: General Conductor Overheating";
    } else if (D10 == 1 && D11 == 0 && D12 == 1 && D13 == 0) {
        return "Case-6: Winding Circulation Currents";
    } else if (D10 == 1 && D11 == 0 && D12 == 2 && D13 == 0) {
        return "Case-7: Core and tank circulating currents, overheating joints";
    } else if (D10 == 0 && D11 == 0 && D12 == 0 && D13 == 1) {
        return "Case-8: Flashover without power follow through";
    }  else if (D10 == 5 && D11 == 0 && (D12 == 1 || D12 == 2) && (D13 == 1 || D13 == 2)) {
        return "Case-9: Arc with power follow through";
    } else if (D10 == 0 && D11 == 0 && D12 == 2 && D13 == 2) {
        return "Case-10: Continuous Sparking to Floating Potential";
    } else if (D10 == 5 && D11 == 0 && D12 == 0 && (D13 == 1 || D13 == 2)) {
        return "Case-8: Flashover without power follow through";
    } else {
        return "N/A";
    }
}  

string c0c02(const DGA &dga){
    if(dga.CO2/dga.CO>11){
        return "Thermal Fault involving cellulose paper < 150 degree C. Check 5H2F (5-hydroxymethyl-2-furaldehyde) and 2FAL (2-furaldehyde) in Furan";
    }
    else if(dga.CO2/dga.CO<3){
        return "Thermal Fault involving cellulose paper > 200 degree C. Check 2FAL (2-furaldehyde)and 5M2F (5-methyl-2-furaldehyde) in Furan ";
    }
    else{
        return "Normal";
    }
}

string Dorenburg(float a, float b, float c, float d){
    
     if(a>1&& b<0.75 && c>0.4 && d<0.3){
        return "Case-0: Thermal Decomposition";
    }
    else if(a<0.1&& b>0.4 && c<0.3){
        return "Case-1: Corona (Low Intensity PD)";
    }
    else if(a<0.1&& b>0.4 && c<0.3){
        return "Case-2: Arcing (High Intensity PD) ";
    }
    else{
        return "N/A";
    }
}

string DUVAL(float first, float second, float third, float sum){
    if(first > 0.98){
        return "PD: Partial Discharge";
    }
    else if(first< 0.8 && second < 0.23 && third >0.80){
        return "D1: Discharge of Low Energy)";
    }
    else if((first< 0.87 && (second >0.23 && second <0.40) && (third> 0.29 && third<0.8)) || (first <0.71 && second > 0.4 && (third >0.29 ||third < 0.6 ))){
        return "D2: Discharge of high energy ";
    }
    else if(second < 0.50 && third < 0.04){
        return "T1: Thermal faults T<300*C";
    }
    else if((second >0.20 || second <0.50) && third<0.04){
        return "T2: Thermal faults 300*C< T <700*C) ";
    }
    else if( second > 0.50 && third<0.15){
        return "T3: Thermal faults T > 700*C) ";
    }
    else{
        return "DT: Thermal and electrical fault";
    }
}
// Main function
int main() {
    DGA dga;

    // Input for DGA
    cout << "Enter CH4 (ppm): ";
    cin >> dga.CH4;
    cout << "Enter C2H6 (ppm): ";
    cin >> dga.C2H6;
    cout << "Enter C2H4 (ppm): ";
    cin >> dga.C2H4;
    cout << "Enter C2H2 (ppm): ";
    cin >> dga.C2H2;
    cout << "Enter H2 (ppm): ";
    cin >> dga.H2;
    cout << "Enter CO (ppm): ";
    cin >> dga.CO;
    cout << "Enter CO2 (ppm): ";
    cin >> dga.CO2;

    // Evaluate and display DGA condition
        cout << endl;
    cout << "Key Gas Result: " << endl << evaluateDGA(dga) << endl;

    // Calculate ratios for IEC evaluation 
    float C10 = dga.C2H2 / dga.C2H4;
    float C11 = dga.CH4 / dga.H2;
    float C12 = dga.C2H4 / dga.C2H6;  
    
    // Corrected calculation
    float C10a = dga.CH4 / dga.H2;
    float C11a = dga.C2H6 / dga.CH4;
    float C12a = dga.C2H4 / dga.C2H6;  
    float C13a = dga.C2H2 / dga.C2H4;
    
    float a = dga.CH4 / dga.H2;
    float b = dga.C2H2 / dga.C2H4;
    float c = dga.C2H6 / dga.C2H2;
    float d = dga.C2H2 / dga.CH4;
    float e= dga.C2H2;
    
    float sum= dga.CH4 + dga.C2H4 + dga.C2H2;
    float first = dga.CH4 /sum;
    float second = dga.C2H4 /sum;
    float third = dga.C2H2 /sum;
    // Evaluate and display IEC condition
    cout << endl;
    cout << "Duval Triangle Condition: " << endl<< DUVAL(first, second, third, sum) << endl;
    cout << endl;
    
    cout << "IEC 599-1978 Condition: " <<endl << evaluateIEC(C10, C11, C12) << endl;
    cout << endl;
        
    cout << "Roger's Ratio Method: " << endl<<  evaluateRoger(C10a, C11a, C12a, C13a) << endl;
    cout << endl;
    
    if(e==0){
        cout << "Doreneburg Ratio: " << endl<<  "N/A" << endl;
    cout << endl;
    }
    else{
    cout << "Doreneburg Ratio: " << endl<<   Dorenburg(a, b, c, d) << endl;
    cout << endl;
    }
    
    cout << "Roger's 4 Ratio Method: " << endl<< evaluateRoger4(C10a, C11a, C12a, C13a) << endl;
     cout << endl;
    
    cout << "CO2/CO Analysis: " << endl << c0c02(dga) << endl;
    cout << endl;
    

    return 0;
}
