function broomAngleCallback(src, ~)
    persistent broomAngle;
    persistent broomAngleDataCount;

    if isempty(broomAngleDataCount)
        broomAngleDataCount = 0;
        assignin('base', 'broomAngle', 0);
    end
    broomAngleDataCount = broomAngleDataCount + 1;

    broomAngleData = read(src, 'oldest');
    cast(uint8(broomAngleData), 'uint16');
    broomAngle(broomAngleDataCount) = ((broomAngleData(2) * 256) + broomAngleData(1)) / 100;
    assignin('base', 'broomAngle', broomAngle);
end