import pickle
import os

def extract_iptm_values(result_pkl_path):
    with open(result_pkl_path, 'rb') as f:
        result_data = pickle.load(f)
    iptm = result_data['iptm']
    return iptm

# Define the path to your result_model_*.pkl file
result_pkl_path = './result_model_1_multimer_v3_pred_0.pkl'

# Extract IPTM values
iptm_values = extract_iptm_values(result_pkl_path)
print(f"IPTM values of model 1: {iptm_values}")

result_pkl_path = './result_model_2_multimer_v3_pred_0.pkl'
iptm_values = extract_iptm_values(result_pkl_path)
print(f"IPTM values of model 2: {iptm_values}")

result_pkl_path = './result_model_3_multimer_v3_pred_0.pkl'
iptm_values = extract_iptm_values(result_pkl_path)
print(f"IPTM values of model 3: {iptm_values}")

result_pkl_path = './result_model_4_multimer_v3_pred_0.pkl'
iptm_values = extract_iptm_values(result_pkl_path)
print(f"IPTM values of model 4: {iptm_values}")

result_pkl_path = './result_model_5_multimer_v3_pred_0.pkl'
iptm_values = extract_iptm_values(result_pkl_path)
print(f"IPTM values of model 5: {iptm_values}")


