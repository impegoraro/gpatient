-- upgrading visits
INSERT INTO Pacientes.Visits 
	SELECT VisitID, NULL AS ParentID, RefPersonID, VisitDate, Sleepiness, Fatigue, Head, Tongue, Urine, Faeces, Menstruation, PulseD, PulseE, Apal, NULL AS Observations 
	FROM Herbanaria.Visits;

INSERT INTO Pacientes.DetailedVisits 
	SELECT VisitID, VisitID, Complaint, Anamnesis, Weight, PhysicalAppearance, Movement, Voice, Smell, Hypertension, Cholesterol, 
		Triglyceride, Diabetes, Transpiration, Dehydration, Anxiety, Irrt, Frustration, Cry, Verm, Ved, Brad, Prt, Aml, Alg, Irritable, Sad, 
		Med, Melan, Hearing, Throat, Scent, Vision, SexualActivity, Body, Abdomen, Circulation, EatingHabits, Prostheses, Surgery, Weight, 
		PreviousTreatment, Pregnancy, Exams, ClinicalAnalysis, Medication, Color, Escle, MedTxt, Treatment 
	FROM Pacientes.Visits2;

INSERT INTO Pacientes.BloodPressure SELECT RefVisitID, High, Low, BPM FROM BloodPressure;
INSERT INTO Pacientes.Pain SELECT PainID,  RefVisitID, Type, Observation, Since FROM Pain;
INSERT INTO Pacientes.Allergies SELECT  AllergyID,RefVisitID,Name,Observation,DeleteDate FROM Allergies;