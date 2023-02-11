%Test application to edit excel files
close all;                                                                          %Close files
clearvars;                                                                          %Clear variables 

BatchNr = '123TestBatch';                                                           %Set batch nr
Filename = sprintf('%s_%s.xlsx', BatchNr, datestr(now,'dd-mm-yyyy'));               %Set Filename


filecheck = exist("testlog.xlsx", "file");                                          %Checks existance of logfile
findnew = exist(sprintf("./logfiles/%s",Filename), "file");                         %checks existance of newfile

if findnew == 0
    status = copyfile("testlog.xlsx", sprintf("./logfiles/%s",Filename),"f");       %Copy's file to logfile folder
end


SNr = 'LPM123456';                                                                  %Set serial number
Date = datetime('now');                                                             %Get date and time from PC
state = 'FAIL';                                                                     %Set the state
matrix = {SNr,Date,state,1,2,3,4,5,6,7, 305.2,627.0, 1001.2, 105.1};                %Test data

if filecheck == 0
    disp("Error:    File not found");                                               %Error message
else
    writecell(matrix, sprintf("./logfiles/%s",Filename), 'WriteMode', 'append');    %Write data to existing file
end