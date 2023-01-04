function readSweepingFrequency(characteristic)
    characteristic.DataAvailableFcn = @sweepingFrequencyCallback;
end