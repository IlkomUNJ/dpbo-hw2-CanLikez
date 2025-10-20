project('dpbo-hw2-canlikez', 'cpp',
  version : '0.1',
  default_options : ['cpp_std=c++17'])

sources = [
  'main/main.cpp',
  'main/bank.cpp',
  'main/bankCustomer.cpp',
  'main/buyer.cpp',
  'main/item.cpp',
  'main/seller.cpp',
  'main/store.cpp',
  'main/transaction.cpp',
  'main/utils.cpp'
]

inc = include_directories('header')

boost_dep = dependency('boost', modules: ['serialization'], required: true)

executable('main',
  sources,
  include_directories : inc,
  dependencies : boost_dep,
  install : true)