% Read the soil moisture channel data from the past two weeks.
% Send an email and tell the user to add water if the value
% is in the lowest 10 %.

% Store the channel ID for the moisture sensor channel.
channelID = 2978485;

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = 'TAKRxk0L0vOtiz+ouKF';
readAPIKey = 'BSJV64I9OLP1QFG5';
% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf("Symptoms of FBD");

% Read the recent data.
data = thingSpeakRead(channelID, 'Fields', 6, 'NumPoints', 1, 'ReadKey', readAPIKey);
% Check to make sure the data was read correctly from the channel.
if (data==1)
    % Email notification
     alertBody = ' One of your cattle has been marked as UNHEALTHY. Immediate attention required!'; 
end
 
 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
