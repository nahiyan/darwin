import '../sass/base.sass'
import { Elm } from '../elm/src/Main.elm'
import '@fortawesome/fontawesome-free/js/all'
import { exec } from 'child_process'
const os = require('os')
const path = require('path')
const toml = require('toml')
const fs = require('fs')

function prepareConfig (extensionName, config, generateRandomModels) {
  const preparedConfig = {}

  if (config[extensionName] === undefined) { config[extensionName] = {} }

  Object.keys(config.common).concat(Object.keys(config[extensionName])).forEach(function (key) {
    preparedConfig[key] = config[extensionName][key] === undefined ? config.common[key] : config[extensionName][key]
  })

  preparedConfig.models_file_path = generateRandomModels === true ? path.resolve('..', '..', 'models', extensionName + '.json') : ''

  return preparedConfig
}

window.addEventListener('DOMContentLoaded', () => {
  try {
    const config = toml.parse(fs.readFileSync(path.resolve('../../config.toml')))
    const extensions = config.extensions

    const app = Elm.Main.init({ node: document.getElementById('main'), flags: extensions })

    app.ports.startExtension.subscribe(function (info) {
      const extensionName = info[0]
      const generateRandomModels = info[1] === 'random'

      const preparedConfig = JSON.stringify(prepareConfig(extensionName, config, generateRandomModels)).replace(/"+/g, '\\"')

      let binaryPath = path.resolve('..', '..', 'extensions', 'bin', extensionName)
      if (os.platform() === 'darwin') { binaryPath = path.join(binaryPath, 'Contents', 'MacOS', extensionName) }

      console.log(preparedConfig)

      exec(`${binaryPath} "${preparedConfig}"`, function (error, stdout, stderr) {
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
