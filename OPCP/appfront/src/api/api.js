// appfront/src/api/api.js
import axiosInstance from './index'

const axios = axiosInstance

export const GetUserList = () => {return axios.get(`http://localhost:8000/api/users/`)}

export const GetContestList = () => {return axios.get('http://localhost:8000/api/contests/')}

export const Register = (username, password) => {
    return axios.post(`http://localhost:8000/api/register/`, {'username': username, 'password': password})
}
export const Login = (username, password) => {
    return axios.post(`http://localhost:8000/api/login/`, {'username': username, 'password': password})
}

export const CreateContest = (name, creator) => {
    return axios.post(`http://localhost:8000/api/contests/`, {'name': name, 'creator': creator})
}