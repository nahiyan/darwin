import '../sass/base.sass'
import { Elm } from '../elm/src/Main.elm'
import '@fortawesome/fontawesome-free/js/all'
import { exec } from 'child_process'

const path = require('path')
const toml = require('toml')
const fs = require('fs')

function prepareConfig (extension, config) {
  const preparedConfig = {}

  if (config[extension] === undefined) { config[extension] = {} }

  Object.keys(config.common).concat(Object.keys(config[extension])).forEach(function (key) {
    preparedConfig[key] = config[extension][key] === undefined ? config.common[key] : config[extension][key]
  })

  return preparedConfig
}

window.addEventListener('DOMContentLoaded', () => {
  try {
    const config = toml.parse(fs.readFileSync(path.resolve('../../config.toml')))
    const extensions = config.extensions

    const app = Elm.Main.init({ node: document.getElementById('main'), flags: extensions })

    app.ports.startExtension.subscribe(function (extension) {
      const preparedConfig = JSON.stringify(prepareConfig(extension, config))

      exec(`../../extensions/bin/${extension} "${preparedConfig}"`, function (error, stdout, stderr) {
        if (error) {
          console.log(`error: ${error.message}`)
          return
        }
        if (stderr) {
          console.log(`stderr: ${stderr}`)
          return
        }
        console.log(stdout)
      })
    })
  } catch (e) {
    console.error('Parsing error on line ' + e.line + ', column ' + e.column +
      ': ' + e.message)
  }
})
