document.getElementById('dgaForm').addEventListener('submit', function (event) {
    event.preventDefault();

    const dga = {
        CH4: parseFloat(document.getElementById('CH4').value),
        C2H6: parseFloat(document.getElementById('C2H6').value),
        C2H4: parseFloat(document.getElementById('C2H4').value),
        C2H2: parseFloat(document.getElementById('C2H2').value),
        H2: parseFloat(document.getElementById('H2').value),
        CO: parseFloat(document.getElementById('CO').value),
        CO2: parseFloat(document.getElementById('CO2').value)
    };

    document.getElementById('keyGasResult').innerText = `Key Gas Result: ${evaluateDGA(dga)}`;

    const C10 = dga.C2H2 / dga.C2H4;
    const C11 = dga.CH4 / dga.H2;
    const C12 = dga.C2H4 / dga.C2H6;

    const C10a = dga.CH4 / dga.H2;
    const C11a = dga.C2H6 / dga.CH4;
    const C12a = dga.C2H4 / dga.C2H6;
    const C13a = dga.C2H2 / dga.C2H4;

    const a = dga.CH4 / dga.H2;
    const b = dga.C2H2 / dga.C2H4;
    const c = dga.C2H6 / dga.C2H2;
    const d = dga.C2H2 / dga.CH4;

    const sum = dga.CH4 + dga.C2H4 + dga.C2H2;
    const first = dga.CH4 / sum;
    const second = dga.C2H4 / sum;
    const third = dga.C2H2 / sum;

    document.getElementById('duvalResult').innerText = `Duval Triangle Condition: ${duvalTriangle(dga)}`;
    document.getElementById('iecResult').innerText = `IEC 599-1978 Condition: ${evaluateIEC(C10, C11, C12)}`;
    document.getElementById('rogerResult').innerText = `Roger's Ratio Method: ${evaluateRoger(C10a, C11a, C12a, C13a)}`;

    if (dga.C2H2 === 0) {
        document.getElementById('dorenburgResult').innerText = 'Dorenburg Ratio: N/A';
    } else {
        document.getElementById('dorenburgResult').innerText = `Dorenburg Ratio: ${Dorenburg(a, b, c, d)}`;
    }

    document.getElementById('roger4Result').innerText = `Roger's 4 Ratio Method: ${evaluateRoger4(C10a, C11a, C12a, C13a)}`;
    document.getElementById('co2coResult').innerText = `CO2/CO Analysis: ${c0c02(dga)}`;
});


function evaluateDGA(dga) {
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

    const ratios = [dga.CH4 / 40, dga.C2H6 / 50, dga.C2H4 / 60, dga.C2H2 / 1, dga.H2 / 60];
    const max_ratio = Math.max(...ratios);

    if (max_ratio === dga.H2 / 60) {
        return "Case-1: Partial Discharges in voids";
    } else if (max_ratio === dga.CH4 / 40) {
        return "Case-2: Sparking < 150°C";
    } else if (max_ratio === dga.C2H6 / 50) {
        return "Case-3: Local Overheating between 150°C and 300°C";
    } else if (max_ratio === dga.C2H4 / 60) {
        return "Case-4: Severe Overheating between 300°C and 700°C";
    } else if (max_ratio === dga.C2H2 / 1) {
        return "Case-5: Arcing > 700°C";
    }

    return "Unknown condition.";
}

function evaluateIEC(C10, C11, C12) {
    const D10 = (C10 < 0.1) ? 0 : (C10 < 1) ? 1 : (C10 < 3) ? 1 : 2;
    const D11 = (C11 < 0.1) ? 1 : (C11 < 1) ? 0 : (C11 < 3) ? 2 : 2;
    const D12 = (C12 < 0.1) ? 0 : (C12 < 1) ? 0 : (C12 < 3) ? 1 : 2;

    if (D10 === 0 && D11 === 0 && D12 === 0) {
        return "Case - 0: No Fault";
    } else if (D10 === 0 && D11 === 1 && D12 === 0) {
        return "Case 1: Low energy Partial Discharges";
    } else if (D10 === 1 && D11 === 1 && D12 === 0) {
        return "Case 2: Low Energy Partial Discharges with tracking";
    } else if ((D10 === 1 || D10 === 2) && D11 === 0 && (D12 === 1 || D12 === 2)) {
        return "Case - 3 : Discharges of Low Energy Density (Sparking)";
    } else if (D10 === 1 && D11 === 0 && D12 === 2) {
        return "Case 4: Discharges of High Energy (Arcing)";
    } else if (D10 === 0 && D11 === 0 && D12 === 1) {
        return "Case 5: Thermal Fault < 150°C - General insulated conductor overheating";
    } else if (D10 === 0 && D11 === 2 && D12 === 0) {
        return "Case 6: Thermal Fault 150°C - 300°C";
    } else if (D10 === 0 && D11 === 2 && D12 === 1) {
        return "Case 7: Thermal Fault 300°C - 700°C";
    } else {
        return "Case 8: Thermal Fault > 700°C";
    }
}

function evaluateRoger(C10a, C11a, C12a, C13a) {
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

function evaluateRoger4(C10a, C11a, C12a, C13a) {
    const D10 = (C10a < 0.1) ? 5 : (C10a < 1) ? 0 : (C10a < 3) ? 2 : 2;
    const D11 = (C10a < 1) ? 0 : (C11a < 1) ? 1 : 1;
    const D12 = (C12a < 1) ? 0 : (C12a < 3) ? 1 : (C12a < 3) ? 2 : 2;
    const D13 = (C13a < 0.5) ? 0 : (C13a < 3) ? 1 : (C13a < 3) ? 2 : 2;

    if (D10 === 0 && D11 === 0 && D12 === 0 && D13 === 0) {
        return "Case-0: Normal";
    } else if (D10 === 5 && D11 === 0 && D12 === 0 && D13 === 0) {
        return "Case-1: Partial Discharge";
    } else if ((D10 === 1 || D10 === 2) && D11 === 0 && D12 === 0 && D13 === 0) {
        return "Case-2: Slight Overheating < 150 C";
    } else if ((D10 === 1 || D10 === 2) && D11 === 1 && D12 === 0 && D13 === 0) {
         return "Case-3: Overheating 150 C - 200 C";
    } else if (D10 === 0 && D11 === 1 && D12 === 0 && D13 === 0) {
        return "Case-4: Overheating 200 C - 300 C";
    } else if (D10 === 0 && D11 === 0 && D12 === 1 && D13 === 0) {
        return "Case-5: General Conductor Overheating";
    } else if (D10 === 1 && D11 === 0 && D12 === 1 && D13 === 0) {
        return "Case-6: Winding Circulation Currents";
    } else if (D10 === 1 && D11 === 0 && D12 === 2 && D13 === 0) {
        return "Case-7: Core and tank circulating currents, overheating joints";
    } else if (D10 === 0 && D11 === 0 && D12 === 0 && D13 === 1) {
        return "Case-8: Flashover without power follow through";
    }  else if (D10 === 5 && D11 === 0 && (D12 === 1 || D12 === 2) && (D13 === 1 || D13 === 2)) {
        return "Case-9: Arc with power follow through";
    } else if (D10 === 0 && D11 === 0 && D12 === 2 && D13 === 2) {
        return "Case-10: Continuous Sparking to Floating Potential";
    } else if (D10 === 5 && D11 === 0 && D12 === 0 && (D13 === 1 || D13 === 2)) {
        return "Case-8: Flashover without power follow through";
    } else {
        return "N/A";
    }
}

function c0c02(dga){
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



function duvalTriangle(dga) {
    let total = dga.CH4 + dga.C2H2 + dga.C2H4;

    if (total === 0) {
        return "Invalid Input: Total concentration of gases cannot be zero.";
    }

    // Normalize the values to get the percentages
    let CH4_percent = (dga.CH4 / total) * 100;
    let C2H2_percent = (dga.C2H2 / total) * 100;
    let C2H4_percent = (dga.C2H4 / total) * 100;

    // Determine the region in Duval's Triangle
    if (C2H2_percent > 29) {
        return "D1: Discharges of high energy (arcing)";
    } else if (C2H4_percent > 48) {
        return "T3: Thermal fault > 700°C";
    } else if (CH4_percent > 98) {
        return "PD: Partial Discharge";
    } else if (C2H2_percent <= 29 && C2H2_percent > 13 && C2H4_percent <= 48 && C2H4_percent > 10) {
        return "D2: Discharges of low energy (sparking)";
    } else if (CH4_percent <= 98 && CH4_percent > 87 && C2H2_percent <= 13 && C2H4_percent <= 10) {
        return "PD: Partial Discharge";
    } else if (C2H2_percent <= 13 && C2H2_percent > 4 && C2H4_percent <= 48 && C2H4_percent > 24 && CH4_percent <= 87 && CH4_percent > 33) {
        return "T2: Thermal fault 300°C - 700°C";
    } else if (C2H4_percent <= 24 && C2H4_percent > 10 && CH4_percent <= 98 && CH4_percent > 33 && C2H2_percent <= 4 && C2H2_percent > 0) {
        return "T1: Thermal fault < 300°C";
    } else {
        return "N/A: Fault type cannot be determined.";
    }
}
