function readBroomAngle(characteristic)
    characteristic.DataAvailableFcn = @broomAngleCallback;
end
