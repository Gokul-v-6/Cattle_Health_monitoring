% Enter your MATLAB Code below
N = 100;
PulseRate = zeros(N,1);
Temperature = zeros(N,1);
MotionLevel = zeros(N,1);
Age = zeros(N,1);
Sex = categorical(zeros(N,1));
Label = categorical(zeros(N,1));

%generate data for training
for i = 1:N
    sex = randi([0, 1]); % 0 = Male, 1 = Female
    age = randi([2, 10]);

    if sex == 1 && rand() < 0.25
        % In ovulation
        pulse = 120 + 10*randn();
        temp = 39.5 + 0.5*randn();
        motion = 6 + 2*randn();
        label = 2;
    else
        if rand() < 0.7
            % Healthy
            pulse = 75 + 10*randn();
            temp = 38 + 0.5*randn();
            motion = 2.5 + 1*randn();
            label = 0;
        else
            % Unhealthy
            pulse = 105 + 15*randn();
            temp = 40 + 0.7*randn();
            motion = 9 + 3*randn();
            label = 1;
        end
    end

    PulseRate(i) = round(pulse,1);
    Temperature(i) = round(temp,1);
    MotionLevel(i) = round(motion,2);
    Age(i) = age;
    Sex(i) = categorical(sex);
    Label(i) = categorical(label);
end

%Random Forest model
T = table(PulseRate, Temperature, MotionLevel, Age, Sex, Label);
rfModel = TreeBagger(20, T, 'Label', 'Method', 'classification');

%live data
channelID = 2978485;
readKey = 'BSJV64I9OLP1QFG5';
data = thingSpeakRead(channelID, 'Fields', [1,2,3,4,5], 'NumPoints', 1, 'ReadKey', readKey);

if isempty(data)
    % test
    pulse = 120;
    temp = 40;
    motion = 6.0;
    age = 5;
    sex = categorical(1);  % Female
else
    pulse = data(1);
    temp = data(2);
    motion = data(3);
    age = data(4);
    sex = categorical(data(5));
end

%Prediction
newInput = table(pulse, temp, motion, age, sex, ...
    'VariableNames', {'PulseRate','Temperature','MotionLevel','Age','Sex'});

[predictedLabel, ~] = predict(rfModel, newInput);
result = string(predictedLabel);

% Write back
writeKey = '0ASEAAAJS5B4N8K3';
thingSpeakWrite(channelID, 'Fields', 6, 'Values', result, 'WriteKey', writeKey);
