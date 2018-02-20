
# # Step #1:
# # line plot of time series
# from pandas import Series
# from matplotlib import pyplot
# # load dataset
# series = Series.from_csv('weather-measurements-pune.csv', header=0)
# # display first 20 rows 
# print(series.head(20))
# # line plot of dataset
# series.plot()
# pyplot.show()
# ---------------------------------------------------------------------------------------------------

# # Step #2:
# # split the dataset
# from pandas import Series
# from matplotlib import pyplot
# # load dataset
# series = Series.from_csv('weather-measurements-pune.csv', header=0)
# split_point = len(series) - 7
# dataset, validation = series[0:split_point], series[split_point:]
# print('Dataset %d, Validation %d' % (len(dataset), len(validation)))
# dataset.to_csv('dataset.csv')        # training dataset
# validation.to_csv('validation.csv')  # test dataset
# ---------------------------------------------------------------------------------------------------

# # Step #3:
# from pandas import Series
# from statsmodels.tsa.arima_model import ARIMA
# import numpy

# # create a differenced series
# def difference(dataset, interval=1):
# 	diff = list()
# 	for i in range(interval, len(dataset)):
# 		value = dataset[i] - dataset[i - interval]
# 		diff.append(value)
# 	return numpy.array(diff)

# # load dataset
# series = Series.from_csv('dataset.csv', header=None)
# # seasonal difference
# X = series.values
# days_in_year = 365
# differenced = difference(X, days_in_year)
# # fit model
# model = ARIMA(differenced, order=(7,0,1))
# model_fit = model.fit(disp=0)
# # print summary of fit model
# print(model_fit.summary())
# ---------------------------------------------------------------------------------------------------

# # Step #4(using forecast() function):
# from pandas import Series
# from statsmodels.tsa.arima_model import ARIMA
# import numpy

# # create a differenced series
# def difference(dataset, interval=1):
# 	diff = list()
# 	for i in range(interval, len(dataset)):
# 		value = dataset[i] - dataset[i - interval]
# 		diff.append(value)
# 	return numpy.array(diff)

# # invert differenced value
# def inverse_difference(history, yhat, interval=1):
# 	return yhat + history[-interval]

# # load dataset
# series = Series.from_csv('dataset.csv', header=None)
# # seasonal difference
# X = series.values
# days_in_year = 365
# differenced = difference(X, days_in_year)
# # fit model
# model = ARIMA(differenced, order=(7,0,1))
# model_fit = model.fit(disp=0)
# # one-step out-of sample forecast
# forecast = model_fit.forecast()[0]
# # invert the differenced forecast to something usable
# forecast = inverse_difference(X, forecast, days_in_year)
# print('Forecast: %f' % forecast)
# ---------------------------------------------------------------------------------------------------

# # Step #5 (using predict() function):
# from pandas import Series
# from statsmodels.tsa.arima_model import ARIMA
# import numpy
# from pandas import datetime

# # create a differenced series
# def difference(dataset, interval=1):
# 	diff = list()
# 	for i in range(interval, len(dataset)):
# 		value = dataset[i] - dataset[i - interval]
# 		diff.append(value)
# 	return numpy.array(diff)

# # invert differenced value
# def inverse_difference(history, yhat, interval=1):
# 	return yhat + history[-interval]

# # load dataset
# series = Series.from_csv('dataset.csv', header=None)
# # seasonal difference
# X = series.values
# days_in_year = 365
# differenced = difference(X, days_in_year)
# # fit model
# model = ARIMA(differenced, order=(7,0,1))
# model_fit = model.fit(disp=0)
# # one-step out of sample forecast
# start_index = len(differenced)
# end_index = len(differenced)
# forecast = model_fit.predict(start=start_index, end=end_index)
# # invert the differenced forecast to something usable
# forecast = inverse_difference(X, forecast, days_in_year)
# print('Forecast: %f' % forecast)
# ---------------------------------------------------------------------------------------------------

# Step #6(Multi-step out of forecast sample):
from pandas import Series
from statsmodels.tsa.arima_model import ARIMA
import numpy

predicted_dataset = []
# create a differenced series
def difference(dataset, interval=1):
	diff = list()
	for i in range(interval, len(dataset)):
		value = dataset[i] - dataset[i - interval]
		diff.append(value)
	return numpy.array(diff)

# invert differenced value
def inverse_difference(history, yhat, interval=1):
	return yhat + history[-interval]

# load dataset
series = Series.from_csv('dataset.csv', header=None)
# seasonal difference
X = series.values
days_in_year = 365
differenced = difference(X, days_in_year)
# fit model
model = ARIMA(differenced, order=(7,0,1))
model_fit = model.fit(disp=0)
# multi-step out-of-sample forecast
forecast = model_fit.forecast(steps=7)[0]
# invert the differenced forecast to something usable
history = [x for x in X]
day = 1
for yhat in forecast:
	inverted = inverse_difference(history, yhat, days_in_year)
	print('Day %d: %f' % (day, inverted))
	history.append(inverted)
	day += 1
	predicted_dataset.append(inverted)

import csv
from matplotlib import pyplot as plt

with open('validation.csv') as csvfile:
	read_csv = csv.reader(csvfile, delimiter=',')

	test_dataset = []
	date = []
	for row in read_csv:
		date.append(row[0])
		test_dataset.append(float(row[1]))

plt.ylim(0,50)
plt.plot(date, test_dataset)
plt.plot(date, predicted_dataset)
plt.show()