close all;
clear;

%% Read data
s = serialport("COM3", 115200);

tp = theaterPlot('XLimit',[-180, 180], 'YLimit',[-180, 180], 'ZLimit',[-180, 180]);
op = orientationPlotter(tp, 'DisplayName', 'Body Axis', 'LocalAxesLength', 100, 'MarkerSize', 10, 'HistoryDepth', 15);
view(-45, 20);

%% Plot data
while(1)
    data = readline(s);
    str = sprintf('%s,', data{:});
    num = sscanf(str, '%g,', [3, inf]).';
    roll = num(1);
    pitch = num(2);
    yaw = num(3);
    
    set(gca, 'Xdir', 'normal', 'Ydir', 'reverse', 'Zdir', 'normal');
    plotOrientation(op, roll, pitch, yaw);
end
