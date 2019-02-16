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

% Find out the first index to select audio value from
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
MyAudioArray = uint16((myRecording(pointer:pointer+1000)+1)*1024/2); 
csvwrite('AudioArray.csv',MyAudioArray');
% Plot the modified waveform.
figure;plot(MyAudioArray);
