#ifndef KRONOS_CONFIGURATION_HPP
#define KRONOS_CONFIGURATION_HPP

#include <QString>
#include <exception>

/**
 * Super class for all exceptions thrown while a requested configuration value
 * is being fetched.
 */
struct ConfigurationException : public std::exception {
	std::string reason;

	ConfigurationException(QString reason) : reason(reason.toStdString()) { }

	const char *what() const noexcept override {
		return reason.c_str();
	}
};

/**
 * Exception thrown if the configuration file could not be opened.
 */
struct FileOpenException : public ConfigurationException {
	FileOpenException(QString path, QString errorDescription)
			: ConfigurationException(
				QString("The configuration file at '%1' could not be opened: %2")
				.arg(path).arg(errorDescription)
			) { }
};

/**
 * Exception thrown if the JSON content of the configuration file could not
 * be parsed.
 */
struct JsonParseException : public ConfigurationException {
	JsonParseException(QString path, QString errorDescription)
			: ConfigurationException(
				QString("The configuration file at '%1' could not be parsed: %2")
				.arg(path).arg(errorDescription)
			) { }
};

/**
 * Exception thrown if the requested key does not exist in the configuration
 * file.
 */
struct InvalidKeyException : public ConfigurationException {
	InvalidKeyException(QString key)
			: ConfigurationException(
				QString("The requested key '%1' could not be found in the configuration file.")
				.arg(key)
			) { }
};

/**
 * Exception thrown if the requested key's value type does not match the specified value type.
 */
struct InvalidValueException : public ConfigurationException {
	InvalidValueException(QString key, QString expectedValue, QString actualValue)
			: ConfigurationException(
				QString("A value of type %1 was requested, but the value of the "
                "requested key '%2' was of type %3.")
				.arg(expectedValue).arg(key).args(actualValue)
			) { }
};

class Configuration {
public:
  /**
   * Get the singleton instance of this class.
   * @return The singleton instance of this class
   */
  static Configuration& getInstance();

  /**
   * Get a string value from the configuration file.
   * @param key The key of the value to get, which is a path through the data
   * tree of the JSON file, separated by dots.
   */
  const QString getString(QString key);

	/**
	 * Get an integer value from the configuration file.
	 * @param key The key of the value to get
	 */
	const int getInteger(QString key);

  /**
   * Get a float value from the configuration file.
   * @param key The key of the value to get
   */
  const float getFloat(QString key);

  /**
   * Get a double value from the configuration file.
   * @param key The key of the value to get
   */
  const double getDouble(QString key);

private:
  /*
   * Hide some things that should not be accessed given this class uses
   * the singleton pattern.
   */
  Configuration();
  Configuration(Configuration const&) = delete;
  void operator=(Configuration const&) = delete;

  /**
   * The path where the configuration file resides.
   */
  static const QString CONFIGURATION_FILE_PATH;
};

#endif
