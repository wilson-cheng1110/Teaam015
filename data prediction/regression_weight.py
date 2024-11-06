import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import numpy as np

def train_weight_model(weights_file='normalized_weights.txt'):
    weights = np.loadtxt(weights_file)
    # Assuming we have features extracted from images (e.g., using CNN)
    features = np.random.rand(len(weights), 10)  # Placeholder for actual features

    X_train, X_test, y_train, y_test = train_test_split(features, weights, test_size=0.2)
    model = LinearRegression()
    model.fit(X_train, y_train)

    print("Model Coefficients:", model.coef_)
    return model

if __name__ == "__main__":
    weight_model = train_weight_model()