function sweepingAngleCallback(src, ~)
    persistent sweepingAngle;
    persistent sweepingAngleDataCount;

    if isempty(sweepingAngleDataCount)
        sweepingAngleDataCount = 0;
        assignin('base', 'sweepingAngle', 0);
    end
    sweepingAngleDataCount = sweepingAngleDataCount + 1;

    sweepingAngleData = read(src, 'oldest');
    cast(uint8(sweepingAngleData), 'uint16');
    sweepingAngle(sweepingAngleDataCount) = ((sweepingAngleData(2) * 256) + sweepingAngleData(1)) / 100;
    assignin('base', 'sweepingAngle', sweepingAngle);
end