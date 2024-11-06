import numpy as np

def normalize_weights(file_path='weights.txt'):
    weights = np.loadtxt(file_path)
    normalized_weights = (weights - np.mean(weights)) / np.std(weights)  # Z-score normalization
    return normalized_weights

if __name__ == "__main__":
    normalized_weights = normalize_weights()
    np.savetxt('normalized_weights.txt', normalized_weights)