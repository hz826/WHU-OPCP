import Vue from 'vue'
import VueRouter from 'vue-router'
import HomeView from '../views/HomeView.vue'
import ContestView from '../views/contest/ContestView.vue'
import LoginView from '../views/login/LoginView.vue'
import LogoutView from '../views/logout/LogoutView.vue'
import RegisterView from '../views/register/RegisterView.vue'
import UserlistView from '../views/userlist/UserlistView.vue'
import JudgelistView from '../views/judgelist/JudgelistView.vue'
import ContestOne from '../components/ContestOne.vue'
import CreateContest from '../components/CreateContest.vue'
import ModifyContest from '../components/ModifyContest.vue'
import JudgelistOne from '../components/JudgelistOne.vue'

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
  {
    path: '/modifycontest/:name',
    name: 'modifycontest',
    component: ModifyContest
  },
  {
    path: '/judgelist',
    name: 'judgelist',
    component: JudgelistView
  },
  {
    path: '/judgelist/:judgeid',
    name: 'judgelistone',
    component: JudgelistOne
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
