// appfront/src/api/api.js
import axiosInstance from './index'
import store from '@/store';

const axios = axiosInstance

export const GetUserList = () => {return axios.get(`http://localhost:8000/api/users/`)}

export const GetUser = (variable) => {
    const apiurl = `http://localhost:8000/api/users/${variable}`
    return axios.get(apiurl)
}

export const GetContestList = () => {return axios.get('http://localhost:8000/api/contests/')}

export const GetDescription = (variable) => {
    const apiurl = `http://localhost:8000/api/contests/${variable}`
    return axios.get(apiurl)
}

export const GetProfile = (variable) => {
    const apiurl = `http://localhost:8000/api/users/${variable}`
    return axios.get(apiurl)
}

export const ModifyDescription = (variable, description) => {
    const apiurl = `http://localhost:8000/api/contests/${variable}`
    return axios.patch(apiurl, {'description': description}, {
        headers: {
            Authorization: 'Bearer ' + store.state.token,
        }})
}

export const GetJudge = (variable) => {
    const apiurl = `http://localhost:8000/api/getjudge/${variable}`
    return axios.get(apiurl)
}

export const GetSubmissions = () => {return axios.get(`http://localhost:8000/api/submissions/`)}

export const GetSubmissionsOne = (variable) => {
    const apiurl = `http://localhost:8000/api/submissions/${variable}`
    return axios.get(apiurl)
}

export const Register = (username, password) => {
    return axios.post(`http://localhost:8000/api/register/`, {'username': username, 'password': password})
}
export const Login = (username, password) => {
    return axios.post(`http://localhost:8000/api/login/`, {'username': username, 'password': password})
}

export const GetToken = (username, password) => {
    return axios.post(`http://localhost:8000/api/token/`, {'username': username, 'password': password})
}

export const CreateContest = (name, creator, description, judger) => {
    return axios.post(`http://localhost:8000/api/contests/`, {
        'name': name, 'creator': creator, 'description': description, 'judger': judger
        }, {
        headers: {
            Authorization: 'Bearer ' + store.state.token,
        }
    })
}

