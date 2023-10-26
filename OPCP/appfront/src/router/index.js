import Vue from 'vue'
import VueRouter from 'vue-router'
import HomeView from '../views/HomeView.vue'
import ContestView from '../views/contest/ContestView.vue'
import LoginView from '../views/login/LoginView.vue'
import LogoutView from '../views/logout/LogoutView.vue'
import RegisterView from '../views/register/RegisterView.vue'
import UserlistView from '../views/userlist/UserlistView.vue'
import ContestOne from '../components/ContestOne.vue'
import CreateContest from '../components/CreateContest.vue'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'home',
    component: HomeView
  },
  {
    path: '/contest',
    name: 'contest',
    component: ContestView
  },
  {
    path: '/register',
    name: 'register',
    component: RegisterView
  },
  {
    path: '/login',
    name: 'login',
    component: LoginView
  },
  {
    path: '/logout',
    name: 'logout',
    component: LogoutView
  },
  {
    path: '/userlist',
    name: 'userlist',
    component: UserlistView
  },
  {
    path: '/contest/:name',
    name: 'contestone',
    component: ContestOne
  },
  {
    path: '/createcontest',
    name: 'createcontest',
    component: CreateContest
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
