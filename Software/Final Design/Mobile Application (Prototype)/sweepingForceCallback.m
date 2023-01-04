function sweepingForceCallback(src, ~)
    persistent sweepingForce;
    persistent forceDataCount;

    if isempty(forceDataCount)
        forceDataCount = 0;
        assignin('base', 'sweepingForce', 0);
    end
    forceDataCount = forceDataCount + 1;

    forceData = read(src, 'oldest');
    cast(uint8(forceData), 'uint16');
    sweepingForce(forceDataCount) = ((forceData(2) * 256) + forceData(1)) / 100;
    assignin('base', 'sweepingForce', sweepingForce);
end