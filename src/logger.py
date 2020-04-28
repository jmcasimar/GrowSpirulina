#!/usr/bin/env python3

# Import Directories
import logging
import coloredlogs
import logging.handlers

class logger:
    def __init__(self):
        # Logging config to file
        logging.basicConfig(level = logging.DEBUG,
                            format = '%(asctime)s %(name)-15s %(levelname)-8s %(message)s',
                            datefmt = '%Y-%m-%d %H:%M:%S',
                            filename = './temp/growMaster.log',
                            filemode = 'a')

        # Define a Handler which writes WARNING messages or higher to the sys.stderr
        console = logging.StreamHandler()
        console.setLevel(logging.INFO)

        # Set a format for console use
        formatter = logging.Formatter('%(asctime)s %(name)-15s %(message)s', '%Y-%m-%d %H:%M:%S')
        # Add Color to log
        colorFormatter = coloredlogs.ColoredFormatter(fmt='%(asctime)s %(name)-15s %(message)s',
                                         datefmt='%Y-%m-%d %H:%M:%S',
                                         level_styles={'debug':{'color':'cyan'},
                                          'info':{'color':'white'},
                                          'warning':{'color':'yellow'},
                                          'error':{'color':'red'},
                                          'critical':{'color':'white', 'bold':True, 'background':'red'}
                                                       },
                                         field_styles={'asctime':{'color':'white'},
                                                       'name':{'color':'green', 'bold':True}
                                                       })

        # Tell the handler to use this format
        console.setFormatter(colorFormatter)

        # Add the handler to the root logger
        logging.getLogger('').addHandler(console)

        # Integrate the main logger with warning Module
        logging.captureWarnings(True)

        # Define loggers for differents areas
        self.logger = logging.getLogger('master')
        self.logger.setLevel(logging.DEBUG)
        self.logger_arduino = logging.getLogger('arduino')

        # Create a file handler for each logger
        handler = logging.handlers.RotatingFileHandler('./temp/master.log', maxBytes = 5000000, backupCount = 3)
        handler.setLevel(logging.DEBUG)
        handler_arduino = logging.handlers.RotatingFileHandler('./temp/arduino.log', maxBytes = 5000000, backupCount = 3)
        handler_arduino.setLevel(logging.DEBUG)

        # Create logging format and linking it to all the handlers
        formatter = logging.Formatter(fmt = '%(asctime)s %(levelname)-8s %(message)s',
                                      datefmt = '%Y-%m-%d %H:%M:%S')
        handler.setFormatter(formatter)
        handler_arduino.setFormatter(formatter)

        # Add the handler to the logger
        self.logger.addHandler(handler)
        self.logger_arduino.addHandler(handler_arduino)
