function sweepingFrequencyCallback(src, ~)
    persistent sweepingFreq;
    persistent freqDataCount;

    if isempty(freqDataCount)
        freqDataCount = 0;
        assignin('base', 'sweepingFreq', 0);
    end
    freqDataCount = freqDataCount + 1;

    freqData = read(src, 'oldest');
    cast(uint8(freqData), 'uint16');
    sweepingFreq(freqDataCount) = ((freqData(2) * 256) + freqData(1)) / 100;
    assignin('base', 'sweepingFreq', sweepingFreq);
end