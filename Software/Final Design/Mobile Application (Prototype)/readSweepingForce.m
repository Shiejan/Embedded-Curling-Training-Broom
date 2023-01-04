function readSweepingForce(characteristic)
    characteristic.DataAvailableFcn = @sweepingForceCallback;
end