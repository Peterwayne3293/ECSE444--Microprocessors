% Record your voice for 5 seconds.
recObj = audiorecorder;
disp('Start speaking.')
recordblocking(recObj, 5);
disp('End of Recording.');

% Play back the recording.
play(recObj);

% Store data in double-precision array.
myRecording = getaudiodata(recObj);
figure; plot(myRecording); % Plot the original waveform.

% Find out th88e first index to select audio value from
flag = 0;
pointer = 0;
while flag == 0
   pointer = pointer + 1;
   if myRecording(pointer) ~= 0
       flag = 1;
   end
end

% cut the area you want and convert it into integers
% crop from pointer to pointer+1000
MyAudioArray = uint16((myRecording(2000:6000)+1)*256/2); 
csvwrite('AudioArray.csv',MyAudioArray');
% Plot the modified waveform.
figure;plot(MyAudioArray);

MyNewTransmit = MyAudioArray;
% Setup the serial port connection in matlab
delete (instrfind);
mcu = serial('COM4','BAUD',115200); %configuration of UART port
mcu.OutputBufferSize=4001;  %8000-bits data size
fopen(mcu); %open mcu port
fwrite(mcu,'x','uint8','async');
pause(0.5);
fwrite(mcu, MyNewTransmit,'uint8','async');
% Set a delay for it to complete the transimission than close the port
pause(1.5);
fclose(mcu);
delete(mcu);
