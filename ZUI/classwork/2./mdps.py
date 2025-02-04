import numpy as np
import matplotlib.pyplot as plt

states = ['Rainy', 'Cloudy', 'Sunny']
actions = ['Umbrella', 'No Umbrella']
rewards = {'Rainy': {'Umbrella': -1, 'No Umbrella': -5}, 
           'Cloudy': {'Umbrella': -1, 'No Umbrella': -1},
           'Sunny': {'Umbrella': -5, 'No Umbrella': -1}
           }
transition = {
    'Rainy': {'Umbrella': {'Rainy': 0.7, 'Cloudy': 0.3, 'Sunny': 0},
            'No Umbrella': {'Rainy': 0.3, 'Cloudy': 0.4, 'Sunny': 0.3}},
    'Cloudy': {'Umbrella': {'Rainy': 0.4, 'Cloudy': 0.6, 'Sunny': 0},
            'No Umbrella': {'Rainy': 0, 'Cloudy': 0.7, 'Sunny': 0.3}},
    'Sunny': {'Umbrella': {'Rainy': 0, 'Cloudy': 0, 'Sunny': 0},
            'No Umbrella': {'Rainy': 0, 'Cloudy': 0.4, 'Sunny': 0.6}}
}

discount_factor = 0.9

def value_iteration():
    V = {s: 0 for s in states}
    while True: 
        new_V = {}
        for s in states:
            values = []
            for a in actions:
                value = rewards[s][a]
                for s2 in states:
                    value += discount_factor * transition[s][a][s2] * V[s2]
                values.append(value)
            new_V[s] = max(values)
            
            if all(abs(V[s] - new_V[s]) < 0.0001 for s in states):
                return new_V
            V = new_V
            
V = value_iteration()