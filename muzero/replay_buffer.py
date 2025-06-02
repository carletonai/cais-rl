import torch
from collections import deque
import numpy as np

class ReplayBuffer:
    def __init__(self, capacity, num_unroll_steps, td_steps):
        self.capacity = capacity
        self.num_unroll_steps = num_unroll_steps
        self.td_steps = td_steps
        self.buffer = deque(maxlen=capacity)
        
    def push(self, game_history):
        self.buffer.append(game_history)
        
    def sample(self, batch_size):
        ## TODO: Prepare a random batch of game histories from the buffer for training
        pass
    
class GameHistory:
    def __init__(self):
        self.states = []
        self.actions = []
        self.policies = []
        self.values = []
        self.rewards = []
        
    def add_step(self, state, action, policy, value, reward):
        self.states.append(state)
        self.actions.append(action)
        self.policies.append(policy)
        self.values.append(value)
        self.rewards.append(reward)
    