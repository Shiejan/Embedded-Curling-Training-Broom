close all;
clear;

%% Read data
filename = "accel_data.csv";
accelData = readtable(filename);

t = table2array(accelData(:, 1));
accelRaw = table2array(accelData(:, 2));
accelFiltered = table2array(accelData(:, 3));
accelFiltered_ML = sgolayfilt(accelRaw, 3, 7);

accelFreq = table2array(accelData(:, 4));
accelFreqValid = ~isnan(accelFreq);
accelFreq = accelFreq(accelFreqValid);

%% Plot data
x_max = length(accelRaw) + 100;

figure(1);
% Raw data
subplot(4, 1, 1);
plot(t, accelRaw);
title("Raw Data");
xlabel("Time (ms)");
ylabel("g Value");

% Filtered data
subplot(4, 1, 2);
plot(t, accelFiltered);
title("C++ Savitzky-Golay Filter");
xlabel("Time (ms)");
ylabel("g Value");

% MATLAB filtered data
subplot(4, 1, 3);
plot(t, accelFiltered_ML);
title("MATLAB Savitzky-Golay Filter");
xlabel("Time (ms)");
ylabel("g Value");

% Frequency
subplot(4, 1, 4);
plot(linspace(t(1), t(end), length(accelFreq)), accelFreq);
title("Sweeping Frequency");
xlabel("Time (ms)");
ylabel("Frequency (Hz)");
