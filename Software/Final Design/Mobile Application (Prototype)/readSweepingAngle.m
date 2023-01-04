function readSweepingAngle(characteristic)
    characteristic.DataAvailableFcn = @sweepingAngleCallback;
end
