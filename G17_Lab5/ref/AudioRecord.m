% % Record your voice for 5 seconds.
% recObj = audiorecorder;
% disp('Start speaking.')
% recordblocking(recObj, 2);
% disp('End of Recording.');
% % 
% % % Play back the recording.
% % play(recObj);
% % 
% % Store data in double-precision array.
% myRecording = getaudiodata(recObj);
% figure; plot(myRecording); % Plot the original waveform.
% 
% cut the area you want and convert it into integers
% MyAudioArray = uint16((myRecording(2000:5999)+1)*1024/2); 
% % csvwrite('AudioArray.csv',MyAudioArray');
% % Plot the modified waveform.
% figure;plot(MyAudioArray);
% 
% MyAudioArray = MyAudioArray/4;

% Setup the serial port connection in matlab
delete (instrfind)
mcu = serial('COM3','BAUD',115200);
mcu.OutputBufferSize=4096;
fopen(mcu);
fwrite(mcu,'x','uint8','async');
pause(0.5);
fwrite(mcu, MyAudioArray,'uint8','async');
% Set a delay for it to complete the transimission than close the port
pause(0.5);
fclose(mcu);
delete(mcu);

