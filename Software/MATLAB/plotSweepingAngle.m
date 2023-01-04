close all;
clear;

%% Read data
s = serialport("COM3", 115200);

tp = theaterPlot('XLimit',[-180, 180],'YLimit',[-180, 180],'ZLimit',[-180, 180]);
op = orientationPlotter(tp, 'DisplayName', 'Body Axis', 'LocalAxesLength', 100);
view(-45, 20);

%% Plot data
while(1)
    roll = 0;
    pitch = 0;
    yaw = str2double(readline(s));
    display(yaw);
    
    set(gca, 'Xdir', 'reverse', 'Ydir', 'reverse', 'Zdir', 'normal');
    plotOrientation(op, roll, pitch, yaw);
end
